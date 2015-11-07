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

#include "Field.h"

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

    inline void read(std::istream& input, bool skipFields) {
      uint32_t totalFieldsSize = readHeader(input);
      if (skipFields)
        input.ignore(totalFieldsSize);
      else
        readFields(input, totalFieldsSize);
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
    inline uint32_t readHeader(std::istream& input) {
      uint32_t totalFieldsSize = 0;

      // Check the input stream is large enough.
      size_t totalHeaderLength = typeLength + sizeof(totalFieldsSize) + sizeof(flags) + sizeof(formId) + 8;

      // Skip the record type.
      input.ignore(typeLength);

      // Read the total fields size.
      input.read(reinterpret_cast<char*>(&totalFieldsSize), sizeof(totalFieldsSize));

      // Read the record flags.
      input.read(reinterpret_cast<char*>(&flags), sizeof(flags));

      // Read the record FormID.
      input.read(reinterpret_cast<char*>(&formId), sizeof(formId));

      // Skip to the end of the header.
      input.ignore(8);

      return totalFieldsSize;
    }

    inline void readFields(std::istream& input, uint32_t totalFieldsSize) {
      std::streampos startingInputPos = input.tellg();
      while ((input.tellg() - startingInputPos) < totalFieldsSize) {
        Field field;
        field.read(input);

        if (field.getType() == "MAST") {
          auto rawData = field.getRawData();
          std::string masterFilename(rawData.first.get(), rawData.second - 1);
          masterFilenames.push_back(masterFilename);
        }
        else if (field.getType() == "SNAM") {
          auto rawData = field.getRawData();
          description = std::string(rawData.first.get(), rawData.second - 1);
        }
      }
    }
  };
}

#endif
