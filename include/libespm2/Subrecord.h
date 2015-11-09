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

#ifndef LIBESPM2_SUBRECORD
#define LIBESPM2_SUBRECORD

#include <string>
#include <fstream>
#include <cstdint>

namespace libespm2 {
  class Subrecord {
  private:
    std::string type;
    std::shared_ptr<char> rawData;
    uint16_t rawDataLength;

    static const int typeLength = 4;
  public:
    inline Subrecord() : rawDataLength(0) {
      type.resize(typeLength);
    }

    inline void read(std::istream& input) {
      input.read(reinterpret_cast<char*>(&type[0]), typeLength);

      input.read(reinterpret_cast<char*>(&rawDataLength), sizeof(rawDataLength));

      rawData = std::shared_ptr<char>(new char[rawDataLength]);
      input.read(rawData.get(), rawDataLength);
    }

    inline std::string getType() const {
      return type;
    }

    inline std::pair<std::shared_ptr<char>, uint16_t> getRawData() const {
      return std::make_pair(rawData, rawDataLength);
    }
  };
}

#endif
