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

#include "libespm2/Plugin.h"

namespace libespm2 {
  namespace tests {
    class PluginTest : public ::testing::Test {
    protected:
      PluginTest() :
        dataPath("./Skyrim/Data"),
        missingPlugin(dataPath / "Blank.missing.esm"),
        emptyFile(dataPath / "EmptyFile.esm"),
        invalidPlugin(dataPath / "NotAPlugin.esm"),
        blankEsm(dataPath / "Blank.esm"),
        blankMasterDependentEsm(dataPath / "Blank - Master Dependent.esm"),
        blankEsp(dataPath / "Blank.esp") {}

      inline virtual void SetUp() {
        ASSERT_FALSE(boost::filesystem::exists(missingPlugin));

        ASSERT_TRUE(boost::filesystem::exists(blankEsm));
        ASSERT_TRUE(boost::filesystem::exists(blankMasterDependentEsm));
        ASSERT_TRUE(boost::filesystem::exists(blankEsp));

        // Write out an empty file.
        std::ofstream out(emptyFile.string());
        out.close();
        ASSERT_TRUE(boost::filesystem::exists(emptyFile));

        // Write out an non-empty, non-plugin file.
        out.open(invalidPlugin.string());
        out << "This isn't a valid plugin file.";
        out.close();
        ASSERT_TRUE(boost::filesystem::exists(invalidPlugin));

        // Make sure each test starts with a new Plugin object.
        plugin = Plugin();
      }

      inline virtual void TearDown() {
        boost::filesystem::remove(emptyFile);
        boost::filesystem::remove(invalidPlugin);
      }

      const boost::filesystem::path dataPath;
      const boost::filesystem::path missingPlugin;
      const boost::filesystem::path emptyFile;
      const boost::filesystem::path invalidPlugin;
      const boost::filesystem::path blankEsm;
      const boost::filesystem::path blankMasterDependentEsm;
      const boost::filesystem::path blankEsp;

      Plugin plugin;
    };

    TEST_F(PluginTest, loadShouldThrowIfPluginDoesNotExist) {
      EXPECT_ANY_THROW(plugin.load(missingPlugin));
    }

    TEST_F(PluginTest, loadShouldNotThrowIfValidPluginExists) {
      EXPECT_NO_THROW(plugin.load(blankEsm));
    }

    TEST_F(PluginTest, loadingAnEmptyFileShouldThrow) {
      EXPECT_ANY_THROW(plugin.load(emptyFile));
    }

    TEST_F(PluginTest, loadingAnInvalidPluginShouldThrow) {
      EXPECT_ANY_THROW(plugin.load(invalidPlugin));
    }

    TEST_F(PluginTest, isValidShouldCorrectlyIdentifyAValidPlugin) {
      EXPECT_TRUE(Plugin::isValid(blankEsm));
    }

    TEST_F(PluginTest, isValidShouldCorrectlyIdentifyAnInvalidPlugin) {
      EXPECT_FALSE(Plugin::isValid(invalidPlugin));
    }

    TEST_F(PluginTest, blankDotEsmShouldHaveCorrectName) {
      ASSERT_NO_THROW(plugin.load(blankEsm));

      EXPECT_EQ(blankEsm.filename().string(), plugin.getName());
    }

    TEST_F(PluginTest, blankDotEspShouldHaveCorrectName) {
      ASSERT_NO_THROW(plugin.load(blankEsp));

      EXPECT_EQ(blankEsp.filename().string(), plugin.getName());
    }

    TEST_F(PluginTest, blankDotEsmShouldBeAMaster) {
      ASSERT_NO_THROW(plugin.load(blankEsm));

      EXPECT_TRUE(plugin.isMasterFile());
    }

    TEST_F(PluginTest, blankDotEspShouldNotBeAMaster) {
      ASSERT_NO_THROW(plugin.load(blankEsp));

      EXPECT_FALSE(plugin.isMasterFile());
    }

    TEST_F(PluginTest, blankDotEsmShouldHaveNoMasters) {
      ASSERT_NO_THROW(plugin.load(blankEsm));

      EXPECT_TRUE(plugin.getMasters().empty());
    }

    TEST_F(PluginTest, blankMasterDependentEsmShouldHaveBlankEsmAsAMaster) {
      ASSERT_NO_THROW(plugin.load(blankMasterDependentEsm));

      std::vector<std::string> masters = plugin.getMasters();
      EXPECT_EQ(1, masters.size());
      EXPECT_EQ(blankEsm.filename().string(), masters[0]);
    }

    TEST_F(PluginTest, blankEsmShouldHaveVersionInDescriptionField) {
      ASSERT_NO_THROW(plugin.load(blankEsm));

      EXPECT_EQ("v5.0", plugin.getDescription());
    }

    TEST_F(PluginTest, blankEspShouldHaveEmptyDescriptionField) {
      ASSERT_NO_THROW(plugin.load(blankEsp));

      EXPECT_TRUE(plugin.getDescription().empty());
    }

    TEST_F(PluginTest, blankEsmShouldHave10RecordsWithTheCorrectFormIds) {
      ASSERT_NO_THROW(plugin.load(blankEsm));

      std::vector<std::string> masters = plugin.getMasters();
      EXPECT_EQ(std::set<FormId>({
        FormId(plugin.getName(), masters, 0xCF0),
        FormId(plugin.getName(), masters, 0xCF1),
        FormId(plugin.getName(), masters, 0xCF2),
        FormId(plugin.getName(), masters, 0xCF3),
        FormId(plugin.getName(), masters, 0xCF4),
        FormId(plugin.getName(), masters, 0xCF5),
        FormId(plugin.getName(), masters, 0xCF6),
        FormId(plugin.getName(), masters, 0xCF7),
        FormId(plugin.getName(), masters, 0xCF8),
        FormId(plugin.getName(), masters, 0xCF9),
      }), plugin.getFormIds());
    }

    TEST_F(PluginTest, blankMasterDependentEsmShouldHave8RecordsWithTheCorrectFormIds) {
      ASSERT_NO_THROW(plugin.load(blankMasterDependentEsm));

      std::vector<std::string> masters = plugin.getMasters();
      EXPECT_EQ(std::set<FormId>({
        FormId(plugin.getName(), masters, 0xCF0),
        FormId(plugin.getName(), masters, 0xCF1),
        FormId(plugin.getName(), masters, 0xCF2),
        FormId(plugin.getName(), masters, 0xCF3),
        FormId(plugin.getName(), masters, 0x01000CEA),
        FormId(plugin.getName(), masters, 0x01000CEB),
        FormId(plugin.getName(), masters, 0x01000CEC),
        FormId(plugin.getName(), masters, 0x01000CED),
      }), plugin.getFormIds());
    }

    TEST_F(PluginTest, loadingBlankEsmsHeaderOnlyShouldReadItsFilenameAsBlankEsm) {
      ASSERT_NO_THROW(plugin.load(blankEsm, true));

      EXPECT_EQ(blankEsm.filename().string(), plugin.getName());
    }

    TEST_F(PluginTest, loadingBlankEsmsHeaderOnlyShouldReadItBeingAMaster) {
      ASSERT_NO_THROW(plugin.load(blankEsm, true));

      EXPECT_TRUE(plugin.isMasterFile());
    }

    TEST_F(PluginTest, loadingBlankEsmsHeaderOnlyShouldReadItsHavingNoMasters) {
      ASSERT_NO_THROW(plugin.load(blankEsm, true));

      EXPECT_TRUE(plugin.getMasters().empty());
    }

    TEST_F(PluginTest, loadingBlankEsmsHeaderOnlyShouldReadAVersionInItsDescriptionField) {
      ASSERT_NO_THROW(plugin.load(blankEsm, true));

      EXPECT_EQ("v5.0", plugin.getDescription());
    }

    TEST_F(PluginTest, loadingBlankEsmsHeaderOnlyShouldSkipItsOtherRecords) {
      ASSERT_NO_THROW(plugin.load(blankEsm, true));

      EXPECT_TRUE(plugin.getFormIds().empty());
    }
  }
}
