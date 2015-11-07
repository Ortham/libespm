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
    std::set<uint32_t> _formIds;

    static const std::string groupType;
    static const int typeLength = 4;
  public:
    inline void read(std::istream& input) {
      uint32_t totalRecordsSize = readHeader(input);
      readRecords(input, totalRecordsSize);
    }

    inline std::set<uint32_t> getRecordFormIds() const {
      return _formIds;
    }

  private:
    inline uint32_t readHeader(std::istream& input) {
      uint32_t groupSize = 0;

      // Check the input stream is large enough.
      size_t totalHeaderLength = typeLength + sizeof(groupSize) + 16;
      if (!isStreamLongEnough(input, totalHeaderLength))
        throw std::runtime_error("Invalid plugin: incomplete group header found.");

      // Read in the record type.
      std::string type;
      type.resize(typeLength);
      input.read(reinterpret_cast<char*>(&type[0]), typeLength);
      if (type != groupType)
        throw std::runtime_error("Invalid plugin: group with type \"" + type + "\" found.");

      // Read the total group size.
      input.read(reinterpret_cast<char*>(&groupSize), sizeof(groupSize));

      // Skip to the end of the header.
      input.seekg(16, std::ios_base::cur);

      return groupSize - 24;
    }

    inline void readRecords(std::istream& input, uint32_t totalRecordsSize) {
      if (!isStreamLongEnough(input, totalRecordsSize))
        throw std::runtime_error("Invalid plugin: incomplete group records found.");

      std::streampos startingInputPos = input.tellg();
      while (input.tellg() - startingInputPos < totalRecordsSize) {
        Record record;
        record.read(input);
        _formIds.insert(record.getFormId());
      }
    }
  };

  const std::string Group::groupType = "GRUP";
}

#endif
