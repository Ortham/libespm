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

#ifndef LIBESPM2_GROUP
#define LIBESPM2_GROUP

#include <cstdint>
#include <fstream>

#include "Record.h"

namespace libespm2 {
  class Group {
  private:
    std::set<uint32_t> formIds;

    static const std::string groupType;
    static const int typeLength = 4;
  public:
    inline void read(std::istream& input, bool skipRecordFields) {
      uint32_t totalRecordsSize = readHeader(input);
      readRecords(input, totalRecordsSize, skipRecordFields);
    }

    inline std::set<uint32_t> getRecordFormIds() const {
      return formIds;
    }

  private:
    inline uint32_t readHeader(std::istream& input) {
      uint32_t groupSize = 0;

      // Check the input stream is large enough.
      size_t totalHeaderLength = typeLength + sizeof(groupSize) + 16;

      // Ignore the group type.
      input.ignore(typeLength);

      // Read the total group size.
      input.read(reinterpret_cast<char*>(&groupSize), sizeof(groupSize));

      // Skip to the end of the header.
      input.ignore(16);

      return groupSize - 24;
    }

    inline void readRecords(std::istream& input, uint32_t totalRecordsSize, bool skipRecordFields) {
      std::streampos startingInputPos = input.tellg();
      while (input.tellg() - startingInputPos < totalRecordsSize) {
        Record record;
        record.read(input, skipRecordFields);
        formIds.insert(record.getFormId());
      }
    }
  };

  const std::string Group::groupType = "GRUP";
}

#endif
