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

#include "PluginTest.h"
#include "FormIdTest.h"

namespace libespm2 {
  void PrintTo(const FormId& value, ::std::ostream* os) {
    *os << "libespm2::FormId(\"" << value.getPluginName() << "\", 0x"
      << std::hex << std::uppercase
      << value.getObjectIndex()
      << std::nouppercase << std::dec
      << ")";
  }

  void PrintTo(GameId value, ::std::ostream* os) {
    if (value == GameId::OBLIVION)
      *os << "GameId::OBLIVION";
    else if (value == GameId::SKYRIM)
      *os << "GameId::SKYRIM";
    else if (value == GameId::FALLOUT3)
      *os << "GameId::FALLOUT3";
    else if (value == GameId::FALLOUTNV)
      *os << "GameId::FALLOUTNV";
    else
      *os << "GameId::MORROWIND";
  }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
