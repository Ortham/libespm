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

#include <gtest/gtest.h>
#include <boost/filesystem.hpp>

#include "libespm/Plugin.h"

namespace libespm {
  namespace tests {
    class PluginTest : public ::testing::TestWithParam<GameId> {
    protected:
      PluginTest() :
        gameId(GetParam()),
        plugin(GetParam()),
        dataPath(getGamePath(GetParam())),
        missingPlugin(dataPath / "Blank.missing.esm"),
        emptyFile(dataPath / "EmptyFile.esm"),
        invalidPlugin(dataPath / "NotAPlugin.esm"),
        nonAsciiPlugin(dataPath / "Русский.esm"),
        blankEsm(dataPath / "Blank.esm"),
        blankMasterDependentEsm(dataPath / "Blank - Master Dependent.esm"),
        blankEsp(dataPath / "Blank.esp"),
        blankBsa(dataPath / "Blank.bsa") {}

      inline virtual void SetUp() {
        ASSERT_FALSE(boost::filesystem::exists(missingPlugin));

        ASSERT_TRUE(boost::filesystem::exists(blankEsm));
        ASSERT_TRUE(boost::filesystem::exists(blankMasterDependentEsm));
        ASSERT_TRUE(boost::filesystem::exists(blankEsp));

        // Write out an empty file.
        boost::filesystem::ofstream out(emptyFile.string());
        out.close();
        ASSERT_TRUE(boost::filesystem::exists(emptyFile));

        // Write out an non-empty, non-plugin file.
        out.open(invalidPlugin.string());
        out << "This isn't a valid plugin file.";
        out.close();
        ASSERT_TRUE(boost::filesystem::exists(invalidPlugin));

        // Copy a valid plugin to be the non-ASCII plugin.
        ASSERT_NO_THROW(boost::filesystem::copy(blankEsm, nonAsciiPlugin));
        ASSERT_TRUE(boost::filesystem::exists(nonAsciiPlugin));

        // Make sure each test starts with a new Plugin object.
        plugin = Plugin(gameId);
      }

      inline virtual void TearDown() {
        ASSERT_NO_THROW(boost::filesystem::remove(emptyFile));
        ASSERT_NO_THROW(boost::filesystem::remove(invalidPlugin));
        ASSERT_NO_THROW(boost::filesystem::remove(nonAsciiPlugin));
      }

      inline boost::filesystem::path getGamePath(GameId game) const {
        if (game == GameId::OBLIVION)
          return "./Oblivion/Data";
        else if (game == GameId::MORROWIND)
          return "./Morrowind/Data Files";
        else
          return "./Skyrim/Data";
      }

      const boost::filesystem::path dataPath;
      const boost::filesystem::path missingPlugin;
      const boost::filesystem::path emptyFile;
      const boost::filesystem::path invalidPlugin;
      const boost::filesystem::path nonAsciiPlugin;
      const boost::filesystem::path blankEsm;
      const boost::filesystem::path blankMasterDependentEsm;
      const boost::filesystem::path blankEsp;
      const boost::filesystem::path blankBsa;

      Plugin plugin;
      GameId gameId;
    };

    // Pass an empty first argument, as it's a prefix for the test instantation,
    // but we only have the one so no prefix is necessary.
    INSTANTIATE_TEST_CASE_P(,
                            PluginTest,
                            ::testing::Values(
                            GameId::SKYRIM,
                            GameId::OBLIVION,
                            GameId::FALLOUT3,
                            GameId::FALLOUTNV,
                            GameId::FALLOUT4,
                            GameId::MORROWIND));

    TEST_P(PluginTest, loadShouldThrowIfPluginDoesNotExist) {
      EXPECT_ANY_THROW(plugin.load(missingPlugin));
    }

    TEST_P(PluginTest, loadShouldNotThrowIfValidPluginExists) {
      EXPECT_NO_THROW(plugin.load(blankEsm));
    }

    TEST_P(PluginTest, loadShouldNotThrowIfAValidPluginWithANonAsciiPathExists) {
      EXPECT_NO_THROW(plugin.load(nonAsciiPlugin));
    }

    TEST_P(PluginTest, loadingAnEmptyFileShouldThrow) {
      EXPECT_ANY_THROW(plugin.load(emptyFile));
    }

    TEST_P(PluginTest, loadingAnInvalidPluginShouldThrow) {
      EXPECT_ANY_THROW(plugin.load(invalidPlugin));
    }

    TEST_P(PluginTest, loadingASkyrimBsaShouldThrow) {
      if (GetParam() != GameId::SKYRIM)
        return;

      EXPECT_ANY_THROW(plugin.load(blankBsa));
    }

    TEST_P(PluginTest, isValidShouldCorrectlyIdentifyAValidPlugin) {
      EXPECT_TRUE(Plugin::isValid(blankEsm, gameId));
    }

    TEST_P(PluginTest, isValidShouldCorrectlyIdentifyAnInvalidPlugin) {
      EXPECT_FALSE(Plugin::isValid(invalidPlugin, gameId));
    }

    TEST_P(PluginTest, isValidShouldCorrectlyIdentifyASkyrimBsaAsAnInvalidPlugin) {
      if (GetParam() != GameId::SKYRIM)
        return;

      EXPECT_FALSE(Plugin::isValid(blankBsa, gameId));
    }

    TEST_P(PluginTest, isValidShouldCorrectlyIdentifyAValidPluginWhenOnlyTheHeaderIsChecked) {
      EXPECT_TRUE(Plugin::isValid(blankEsm, gameId, true));
    }

    TEST_P(PluginTest, isValidShouldCorrectlyIdentifyAnInvalidPluginWhenOnlyTheHeaderIsChecked) {
      EXPECT_FALSE(Plugin::isValid(invalidPlugin, gameId, true));
    }

    TEST_P(PluginTest, isValidShouldCorrectlyIdentifyASkyrimBsaAsAnInvalidPluginWhenOnlyTheHeaderIsChecked) {
      if (GetParam() != GameId::SKYRIM)
        return;

      EXPECT_FALSE(Plugin::isValid(blankBsa, gameId, true));
    }

    TEST_P(PluginTest, blankDotEsmShouldHaveCorrectName) {
      ASSERT_NO_THROW(plugin.load(blankEsm));

      EXPECT_EQ(blankEsm.filename().string(), plugin.getName());
    }

    TEST_P(PluginTest, blankDotEspShouldHaveCorrectName) {
      ASSERT_NO_THROW(plugin.load(blankEsp));

      EXPECT_EQ(blankEsp.filename().string(), plugin.getName());
    }

    TEST_P(PluginTest, blankDotEsmShouldBeAMaster) {
      ASSERT_NO_THROW(plugin.load(blankEsm));

      EXPECT_TRUE(plugin.isMasterFile());
    }

    TEST_P(PluginTest, blankDotEspShouldNotBeAMaster) {
      ASSERT_NO_THROW(plugin.load(blankEsp));

      EXPECT_FALSE(plugin.isMasterFile());
    }

    TEST_P(PluginTest, blankDotEsmShouldHaveNoMasters) {
      ASSERT_NO_THROW(plugin.load(blankEsm));

      EXPECT_TRUE(plugin.getMasters().empty());
    }

    TEST_P(PluginTest, blankMasterDependentEsmShouldHaveBlankEsmAsAMaster) {
      ASSERT_NO_THROW(plugin.load(blankMasterDependentEsm));

      std::vector<std::string> masters = plugin.getMasters();
      EXPECT_EQ(1, masters.size());
      EXPECT_EQ(blankEsm.filename().string(), masters[0]);
    }

    TEST_P(PluginTest, blankEsmShouldHaveVersionInDescription) {
      ASSERT_NO_THROW(plugin.load(blankEsm));

      EXPECT_EQ("v5.0", plugin.getDescription());
    }

    TEST_P(PluginTest, blankEspShouldHaveANonAsciiDescription) {
      ASSERT_NO_THROW(plugin.load(blankEsp));

      EXPECT_EQ("€ƒŠ", plugin.getDescription());
    }

    TEST_P(PluginTest, blankMasterDependentEsmShouldHaveAnEmptyDescription) {
      ASSERT_NO_THROW(plugin.load(blankMasterDependentEsm));

      EXPECT_TRUE(plugin.getDescription().empty());
    }

    TEST_P(PluginTest, gettingFormIdsShouldThrowForMorrowindButNotForOtherGames) {
      if (gameId == GameId::MORROWIND)
        EXPECT_THROW(plugin.getFormIds(), std::domain_error);
      else
        EXPECT_NO_THROW(plugin.getFormIds());
    }

    TEST_P(PluginTest, blankEsmShouldHave10RecordsWithTheCorrectFormIdsForNonMorrowindGames) {
      // No point testing for Morrowind, we know it throws from another test.
      if (gameId == GameId::MORROWIND)
        return;

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

    TEST_P(PluginTest, gettingTheFormIdsOfAGhostedBlankEsmShouldTrimTheGhostExtensionInTheFormIdObject) {
      // No point testing for Morrowind, we know it throws from another test.
      if (gameId == GameId::MORROWIND)
        return;

      ASSERT_NO_THROW(boost::filesystem::copy(blankEsm, blankEsm.string() + ".ghost"));

      EXPECT_NO_THROW(plugin.load(blankEsm.string() + ".ghost"));

      std::vector<std::string> masters = plugin.getMasters();
      EXPECT_EQ(std::set<FormId>({
        FormId(blankEsm.filename().string(), masters, 0xCF0),
        FormId(blankEsm.filename().string(), masters, 0xCF1),
        FormId(blankEsm.filename().string(), masters, 0xCF2),
        FormId(blankEsm.filename().string(), masters, 0xCF3),
        FormId(blankEsm.filename().string(), masters, 0xCF4),
        FormId(blankEsm.filename().string(), masters, 0xCF5),
        FormId(blankEsm.filename().string(), masters, 0xCF6),
        FormId(blankEsm.filename().string(), masters, 0xCF7),
        FormId(blankEsm.filename().string(), masters, 0xCF8),
        FormId(blankEsm.filename().string(), masters, 0xCF9),
      }), plugin.getFormIds());

      ASSERT_NO_THROW(boost::filesystem::remove(blankEsm.string() + ".ghost"));
    }

    TEST_P(PluginTest, blankMasterDependentEsmShouldHave8RecordsWithTheCorrectFormIdsForNonMorrowindGames) {
      // No point testing for Morrowind, we know it throws from another test.
      if (gameId == GameId::MORROWIND)
        return;

      ASSERT_NO_THROW(plugin.load(blankMasterDependentEsm));

      std::vector<std::string> masters = plugin.getMasters();
      EXPECT_EQ(std::set<FormId>({
        FormId(blankEsm.filename().string(), masters, 0xCF0),
        FormId(blankEsm.filename().string(), masters, 0xCF1),
        FormId(blankEsm.filename().string(), masters, 0xCF2),
        FormId(blankEsm.filename().string(), masters, 0xCF3),
        FormId(plugin.getName(), masters, 0x01000CEA),
        FormId(plugin.getName(), masters, 0x01000CEB),
        FormId(plugin.getName(), masters, 0x01000CEC),
        FormId(plugin.getName(), masters, 0x01000CED),
      }), plugin.getFormIds());
    }

    TEST_P(PluginTest, loadingBlankEsmsHeaderOnlyShouldReadItsFilenameAsBlankEsm) {
      ASSERT_NO_THROW(plugin.load(blankEsm, true));

      EXPECT_EQ(blankEsm.filename().string(), plugin.getName());
    }

    TEST_P(PluginTest, loadingBlankEsmsHeaderOnlyShouldReadItBeingAMaster) {
      ASSERT_NO_THROW(plugin.load(blankEsm, true));

      EXPECT_TRUE(plugin.isMasterFile());
    }

    TEST_P(PluginTest, loadingBlankEsmsHeaderOnlyShouldReadItsHavingNoMasters) {
      ASSERT_NO_THROW(plugin.load(blankEsm, true));

      EXPECT_TRUE(plugin.getMasters().empty());
    }

    TEST_P(PluginTest, loadingBlankEsmsHeaderOnlyShouldReadAVersionInItsDescription) {
      ASSERT_NO_THROW(plugin.load(blankEsm, true));

      EXPECT_EQ("v5.0", plugin.getDescription());
    }

    TEST_P(PluginTest, loadingBlankEsmsHeaderOnlyShouldSkipItsOtherRecords) {
      // No point testing for Morrowind, we know it throws from another test.
      if (gameId == GameId::MORROWIND)
        return;

      ASSERT_NO_THROW(plugin.load(blankEsm, true));

      EXPECT_TRUE(plugin.getFormIds().empty());
    }

    TEST_P(PluginTest, gettingTheRecordAndGroupCountShouldWorkWhenLoadingPlugin) {
      ASSERT_NO_THROW(plugin.load(blankEsm));

      if (gameId == GameId::MORROWIND)
        EXPECT_EQ(10, plugin.getRecordAndGroupCount());
      else if (gameId == GameId::OBLIVION)
        EXPECT_EQ(14, plugin.getRecordAndGroupCount());
      else
        EXPECT_EQ(15, plugin.getRecordAndGroupCount());
    }

    TEST_P(PluginTest, gettingTheRecordAndGroupCountShouldWorkWhenLoadingPluginHeaderOnly) {
      ASSERT_NO_THROW(plugin.load(blankEsm, true));

      if (gameId == GameId::MORROWIND)
        EXPECT_EQ(10, plugin.getRecordAndGroupCount());
      else if (gameId == GameId::OBLIVION)
        EXPECT_EQ(14, plugin.getRecordAndGroupCount());
      else
        EXPECT_EQ(15, plugin.getRecordAndGroupCount());
    }
  }
}
