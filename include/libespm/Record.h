/*
 * This file is part of libespm
 *
 * Copyright (C) 2015 Oliver Hamlet
 *
 * libespm is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libespm is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libespm. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LIBESPM_RECORD
#define LIBESPM_RECORD

#include <string>
#include <fstream>
#include <cstdint>

#include "Subrecord.h"

namespace libespm {
  class Record {
  private:
    std::string type;
    uint32_t flags;
    uint32_t formId;
    std::vector<Subrecord> subrecords;

    static const int typeLength = 4;
  public:
    Record() : flags(0), formId(0), type(typeLength, '\0') {}

    inline void read(std::istream& input, GameId gameId, bool skipSubrecords) {
      subrecords.clear();

      uint32_t totalSubrecordsSize = readHeader(input, gameId);
      if (skipSubrecords)
        input.ignore(totalSubrecordsSize);
      else
        readSubrecords(input, gameId, totalSubrecordsSize);
    }

    inline std::string getType() const {
      return type;
    }

    inline uint32_t getFlags() const {
      return flags;
    }

    inline const std::vector<Subrecord>& getSubrecords() const {
      return subrecords;
    }

    inline uint32_t getFormId() const {
      return formId;
    }

  private:
    inline uint32_t readHeader(std::istream& input, GameId gameId) {
      uint32_t totalSubrecordsSize = 0;

      // Read in the record type.
      input.read(&type[0], typeLength);

      // Read the total subrecords size.
      input.read(reinterpret_cast<char*>(&totalSubrecordsSize), sizeof(totalSubrecordsSize));

      if (gameId == GameId::MORROWIND)
        input.ignore(4);  // Skip the next 4 (unknown) bytes.

      // Read the record flags.
      input.read(reinterpret_cast<char*>(&flags), sizeof(flags));

      // Read the record FormID. Morrowind doesn't have FormIDs.
      if (gameId != GameId::MORROWIND) {
        input.read(reinterpret_cast<char*>(&formId), sizeof(formId));

        // Skip to the end of the header. Header length varies by game.
        input.ignore(getHeaderLengthToSkip(gameId));
      }

      return totalSubrecordsSize;
    }

    inline void readSubrecords(std::istream& input, GameId gameId, uint32_t totalSubrecordsSize) {
      // Subrecords can be compressed, in which case they must be uncompressed
      // before they can be read. However, there's currently no need to
      // actually read such subrecords, so just skip over them.
      if (areFieldsCompressed()) {
        input.ignore(totalSubrecordsSize);
        return;
      }

      const std::streampos startingInputPos = input.tellg();
      uint32_t largeSubrecordSize = 0;
      while (input.good() && input.tellg() - startingInputPos < totalSubrecordsSize) {
        Subrecord subrecord;
        subrecord.read(input, gameId, largeSubrecordSize);

        if (subrecord.getType() == "XXXX") {
          largeSubrecordSize = *reinterpret_cast<uint32_t*>(subrecord.getRawData().first.get());
        } else {
          largeSubrecordSize = 0;
          subrecords.push_back(subrecord);
        }
      }
    }

    inline size_t getHeaderLengthToSkip(GameId game) const {
      if (game == GameId::OBLIVION)
        return 4;
      else
        return 8;
    }

    inline bool areFieldsCompressed() const {
      return (flags & 0x00040000) != 0;
    }
  };
}

#endif
