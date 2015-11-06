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

#include <gtest/gtest.h>
#include <boost/filesystem.hpp>

#include "libespm2/SkyrimPlugin.h"

namespace libespm2 {
  namespace tests {

    using libespm2::SkyrimPlugin;

    class SkyrimPluginTest : public ::testing::Test {
      protected:
      SkyrimPluginTest() :
        dataPath("./Skyrim/Data"),
        missingPlugin(dataPath / "Blank.missing.esm"),
        emptyFile(dataPath / "EmptyFile.esm"),
        blankEsm(dataPath / "Blank.esm"),
        blankEsp(dataPath / "Blank.esp")
        {}

      inline virtual void SetUp() {
        ASSERT_FALSE(boost::filesystem::exists(missingPlugin));

        ASSERT_TRUE(boost::filesystem::exists(blankEsm));
        ASSERT_TRUE(boost::filesystem::exists(blankEsp));

        // Write out an empty file.
        std::ofstream out(emptyFile.string());
        out.close();
        ASSERT_TRUE(boost::filesystem::exists(emptyFile));
      }

      inline virtual void TearDown() {
        boost::filesystem::remove(emptyFile);
      }

      const boost::filesystem::path dataPath;
      const boost::filesystem::path missingPlugin;
      const boost::filesystem::path emptyFile;
      const boost::filesystem::path blankEsm;
      const boost::filesystem::path blankEsp;
    };

    TEST_F(SkyrimPluginTest, loadShouldThrowIfPluginDoesNotExist) {
      SkyrimPlugin plugin;
      EXPECT_ANY_THROW(plugin.load(missingPlugin));
    }

    TEST_F(SkyrimPluginTest, loadShouldNotThrowIfPluginExists) {
      SkyrimPlugin plugin;
      EXPECT_NO_THROW(plugin.load(blankEsm));
    }

    TEST_F(SkyrimPluginTest, loadingAnEmptyFileShouldThrow) {
      SkyrimPlugin plugin;
      EXPECT_ANY_THROW(plugin.load(emptyFile));
    }

    TEST_F(SkyrimPluginTest, loadedBlankDotEsmShouldHaveCorrectName) {
      SkyrimPlugin plugin;
      ASSERT_NO_THROW(plugin.load(blankEsm));

      EXPECT_EQ(blankEsm.filename().string(), plugin.getName());
    }

    TEST_F(SkyrimPluginTest, loadedBlankDotEspShouldHaveCorrectName) {
      SkyrimPlugin plugin;
      ASSERT_NO_THROW(plugin.load(blankEsp));

      EXPECT_EQ(blankEsp.filename().string(), plugin.getName());
    }

  }
}
