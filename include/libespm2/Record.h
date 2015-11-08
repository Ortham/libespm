/*
 * This file is part of libespm2
 *
 * Copyright (C) 2015 Oliver Hamlet
 *
 * libespm2 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libespm2 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libespm2. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LIBESPM2_RECORD
#define LIBESPM2_RECORD

#include <string>
#include <fstream>
#include <cstdint>

#include "Subrecord.h"
#include "GameId.h"

namespace libespm2 {
  class Record {
  private:
    uint32_t flags;
    uint32_t formId;

    std::vector<std::string> masterFilenames;
    std::string description;

    static const int typeLength = 4;
  public:
    Record() : flags(0), formId(0) {}

    inline void read(std::istream& input, GameId gameId, bool skipSubrecords) {
      // Header length varies by game.
      size_t headerLengthToSkip = getHeaderLengthToSkip(gameId);

      uint32_t totalSubrecordsSize = readHeader(input, headerLengthToSkip);
      if (skipSubrecords)
        input.ignore(totalSubrecordsSize);
      else
        readSubrecords(input, totalSubrecordsSize);
    }

    inline bool isMasterFlagSet() const {
      return (flags & 0x00000001) != 0;
    }

    inline std::vector<std::string> getMasters() const {
      return masterFilenames;
    }

    inline std::string getDescription() const {
      return description;
    }

    inline uint32_t getFormId() const {
      return formId;
    }

  private:
    inline uint32_t readHeader(std::istream& input, size_t headerLengthToSkip) {
      uint32_t totalSubrecordsSize = 0;

      // Skip the record type.
      input.ignore(typeLength);

      // Read the total subrecords size.
      input.read(reinterpret_cast<char*>(&totalSubrecordsSize), sizeof(totalSubrecordsSize));

      // Read the record flags.
      input.read(reinterpret_cast<char*>(&flags), sizeof(flags));

      // Read the record FormID.
      input.read(reinterpret_cast<char*>(&formId), sizeof(formId));

      // Skip to the end of the header.
      input.ignore(headerLengthToSkip);

      return totalSubrecordsSize;
    }

    inline void readSubrecords(std::istream& input, uint32_t totalSubrecordsSize) {
      std::streampos startingInputPos = input.tellg();
      while (input.good() && input.tellg() - startingInputPos < totalSubrecordsSize) {
        Subrecord subrecord;
        subrecord.read(input);

        if (subrecord.getType() == "MAST") {
          auto rawData = subrecord.getRawData();
          std::string masterFilename(rawData.first.get(), rawData.second - 1);
          masterFilenames.push_back(masterFilename);
        }
        else if (subrecord.getType() == "SNAM") {
          auto rawData = subrecord.getRawData();
          description = std::string(rawData.first.get(), rawData.second - 1);
        }
      }
    }

    inline size_t getHeaderLengthToSkip(GameId game) const {
      if (game == GameId::OBLIVION)
        return 4;
      else
        return 8;
    }
  };
}

#endif
