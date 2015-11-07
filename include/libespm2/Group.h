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
    uint32_t groupSize;
    std::set<uint32_t> _formIds;

    static const int typeLength = 4;
  public:
    inline Group() : groupSize(0) {}

    inline void read(std::istream& input) {
      readHeader(input);
      readRecords(input);
    }

    inline void readHeader(std::istream& input) {
      // Check the input stream is large enough.
      size_t totalHeaderLength = 4 + sizeof(groupSize) + 16;
      if (!streamIsLongEnough(input, totalHeaderLength))
        throw std::runtime_error("Invalid plugin: incomplete group header found.");

      // Read in the record type.
      char type[typeLength];
      input.read(type, typeLength);
      if (memcmp(type, "GRUP", typeLength) != 0)
        throw std::runtime_error("Invalid plugin: group with type \"" + std::string(type, 4) + "\" found.");

      // Read the total group size.
      input.read(reinterpret_cast<char*>(&groupSize), sizeof(groupSize));

      // Skip to the end of the header.
      input.seekg(16, std::ios_base::cur);
    }

    inline void readRecords(std::istream& input) {
      uint32_t totalRecordsSize = groupSize - 24;
      if (!streamIsLongEnough(input, totalRecordsSize))
        throw std::runtime_error("Invalid plugin: incomplete group records found.");

      size_t bytesRead = 0;
      while (bytesRead < totalRecordsSize) {
        Record record;
        std::streampos beforePos = input.tellg();
        record.read(input);
        _formIds.insert(record.getFormId());
        bytesRead += input.tellg() - beforePos;
      }
    }

    inline std::set<uint32_t> getRecordFormIds() const {
      return _formIds;
    }

    inline bool streamIsLongEnough(std::istream& input, size_t expectedMinimumRelativeLength) {
      std::streampos currentPos = input.tellg();

      input.seekg(0, std::ios_base::end);
      std::streampos endPos = input.tellg();

      input.seekg(currentPos);

      return (endPos - currentPos - expectedMinimumRelativeLength) >= 0;
    }
  };
}

#endif
