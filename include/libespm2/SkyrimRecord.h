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

#include <string>
#include <fstream>
#include <cstdint>

namespace libespm2 {
  class SkyrimRecord {
  private:
    uint32_t flags;
    uint32_t totalFieldsSize;

    std::vector<std::string> masterFilenames;

    static const int typeLength = 4;
  public:
    SkyrimRecord() : flags(0), totalFieldsSize(0) {}

    inline void read(std::istream& input) {
      readHeader(input);
      readFields(input);
    }

    inline void readHeader(std::istream& input) {
      // Check the input stream is large enough.
      size_t totalHeaderLength = 4 + sizeof(totalFieldsSize) + sizeof(flags) + 12;
      if (!streamIsLongEnough(input, totalHeaderLength))
        throw std::runtime_error("File is not large enough to be a valid plugin.");

      // Read in the record type.
      char type[typeLength];
      input.read(type, typeLength);
      if (memcmp(type, "TES4", typeLength) != 0)
        throw std::runtime_error("Not a valid plugin file.");

      // Read the total fields size.
      input.read(reinterpret_cast<char*>(&totalFieldsSize), sizeof(totalFieldsSize));

      // Read the record flags.
      input.read(reinterpret_cast<char*>(&flags), sizeof(flags));

      // Skip to the end of the header.
      input.seekg(12, std::ios_base::cur);
    }

    inline void readFields(std::istream& input) {
      if (!streamIsLongEnough(input, totalFieldsSize))
        throw std::runtime_error("File is not large enough to be a valid plugin.");

      uint32_t bytesRead = 0;
      while (bytesRead < totalFieldsSize) {
        char type[typeLength];
        input.read(type, typeLength);
        bytesRead += typeLength;

        if (memcmp(type, "MAST", 4) == 0) {
          // A master filename, store it.
          uint16_t dataLength = 0;
          input.read(reinterpret_cast<char*>(&dataLength), sizeof(dataLength));
          bytesRead += sizeof(dataLength);

          std::string masterFilename;
          masterFilename.resize(dataLength - 1);
          input.read(reinterpret_cast<char*>(&masterFilename[0]), dataLength - 1);
          bytesRead += dataLength;

          masterFilenames.push_back(masterFilename);
        }
      }
    }

    inline bool isMasterFlagSet() const {
      return (flags & 0x00000001) != 0;
    }

    inline std::vector<std::string> getMasters() const {
      return masterFilenames;
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
