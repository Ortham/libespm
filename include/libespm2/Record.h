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

    inline void read(std::istream& input) {
      uint32_t totalFieldsSize = readHeader(input);
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
      if (!streamIsLongEnough(input, totalHeaderLength))
        throw std::runtime_error("File is not large enough to be a valid plugin.");

      // Skip the record type.
      input.seekg(typeLength, std::ios_base::cur);

      // Read the total fields size.
      input.read(reinterpret_cast<char*>(&totalFieldsSize), sizeof(totalFieldsSize));

      // Read the record flags.
      input.read(reinterpret_cast<char*>(&flags), sizeof(flags));

      // Read the record FormID.
      input.read(reinterpret_cast<char*>(&formId), sizeof(formId));

      // Skip to the end of the header.
      input.seekg(8, std::ios_base::cur);

      return totalFieldsSize;
    }

    inline void readFields(std::istream& input, uint32_t totalFieldsSize) {
      if (!streamIsLongEnough(input, totalFieldsSize))
        throw std::runtime_error("File is not large enough to be a valid plugin.");

      std::streampos startingInputPos = input.tellg();
      while ((input.tellg() - startingInputPos) < totalFieldsSize) {
        readField(input);
      }
    }

    inline void readField(std::istream& input) {
      std::string type;
      type.resize(typeLength);
      input.read(reinterpret_cast<char*>(&type[0]), typeLength);

      uint16_t dataLength = 0;
      input.read(reinterpret_cast<char*>(&dataLength), sizeof(dataLength));

      if (type == "MAST") {
        // A master filename, store it.
        std::string masterFilename;
        masterFilename.resize(dataLength);
        input.read(reinterpret_cast<char*>(&masterFilename[0]), dataLength);
        masterFilename.resize(dataLength - 1);

        masterFilenames.push_back(masterFilename);
      }
      else if (type == "SNAM") {
        // The description field, store it.
        description.resize(dataLength);
        input.read(reinterpret_cast<char*>(&description[0]), dataLength);
        description.resize(dataLength - 1);
      }
      else {
        input.seekg(dataLength, std::ios_base::cur);
      }
    }

    inline bool streamIsLongEnough(std::istream& input, size_t expectedMinimumRelativeLength) {
      std::streampos currentPos = input.tellg();

      input.seekg(0, std::ios_base::end);
      std::streampos endPos = input.tellg();

      input.seekg(currentPos);

      return (endPos - currentPos) >= expectedMinimumRelativeLength;
    }
  };
}

#endif
