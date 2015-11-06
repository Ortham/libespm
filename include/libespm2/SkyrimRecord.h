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
  public:
    inline void read(std::istream& input) {
      readHeader(input);
    }

    inline void readHeader(std::istream& input) {
      // Read in the record type.
      char type[4];
      input.read(type, 4);
      if (memcmp(type, "TES4", 4) != 0)
        throw std::runtime_error("Not a valid plugin file.");

      // Skip a field.
      input.seekg(4, std::ios_base::cur);

      // Read the record flags.
      input.read(reinterpret_cast<char*>(&flags), 4);
    }

    inline bool isMasterFlagSet() const {
      return (flags & 0x00000001) != 0;
    }
  };
}
