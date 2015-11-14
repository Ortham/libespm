/*
 * This file is part of libespm
 *
 * Copyright (C) 2015 Oliver Hamlet
 *
 * libespm is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libespm is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libespm. If not, see <http://www.gnu.org/licenses/>.
 */

#include "PluginTest.h"
#include "FormIdTest.h"

#include <codecvt>

namespace libespm {
  void PrintTo(const FormId& value, ::std::ostream* os) {
    *os << "libespm::FormId(\"" << value.getPluginName() << "\", 0x"
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
    else if (value == GameId::FALLOUT4)
      *os << "GameId::FALLOUT4";
    else
      *os << "GameId::MORROWIND";
  }
}

int main(int argc, char **argv) {
  // Set the locale to get UTF-8 conversions working correctly.
  std::locale::global(std::locale(std::locale(), new std::codecvt_utf8_utf16<wchar_t>));
  boost::filesystem::path::imbue(std::locale());

  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
