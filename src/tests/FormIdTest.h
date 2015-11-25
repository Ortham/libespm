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

#include <algorithm>

#include <gtest/gtest.h>
#include <boost/filesystem.hpp>

#include "libespm/FormId.h"

namespace libespm {
  namespace tests {
    class FormIdTest : public ::testing::Test {
    protected:
      std::string parentPluginName;
      std::vector<std::string> masters;

      inline virtual void SetUp() {
        parentPluginName = "plugin1";
        masters = std::vector<std::string>({
          "plugin2",
          "plugin3"
        });
      }
    };

    TEST_F(FormIdTest, objectIndexForFormIdsWithAZeroModIndexShouldBeEqualToOriginalFormId) {
      FormId formId(parentPluginName, masters, 0x01);
      EXPECT_EQ(0x01, formId.getObjectIndex());
    }

    TEST_F(FormIdTest, objectIndexForFormIdsWithANonZeroModIndexShouldBeEqualToLast3BytesOfOriginalFormId) {
      FormId formId(parentPluginName, masters, 0x01000001);
      EXPECT_EQ(0x01, formId.getObjectIndex());
    }

    TEST_F(FormIdTest, zeroModIndexWithNoMastersShouldUseParentPluginAsPlugin) {
      FormId formId(parentPluginName, std::vector<std::string>(), 0x05000001);

      EXPECT_EQ(parentPluginName, formId.getPluginName());
    }

    TEST_F(FormIdTest, nonZeroModIndexWithNoMastersShouldUseParentPluginAsPlugin) {
      FormId formId(parentPluginName, std::vector<std::string>(), 0x05000001);

      EXPECT_EQ(parentPluginName, formId.getPluginName());
    }

    TEST_F(FormIdTest, pluginForFormIdWithAZeroModIndexAndAtLeastOneMasterShouldBeTheFirstMaster) {
      FormId formId(parentPluginName, masters, 0x01);
      EXPECT_EQ(masters[0], formId.getPluginName());
    }

    TEST_F(FormIdTest, pluginForFormIdWithAModIndexLessThanTheNumberOfMastersShouldBeTheMasterAtThatIndex) {
      FormId formId(parentPluginName, masters, 0x01000001);
      EXPECT_EQ(masters[1], formId.getPluginName());
    }

    TEST_F(FormIdTest, pluginForFormIdWithAModIndexGreaterThanTheLastMastersIndexShouldBeTheParentPlugin) {
      FormId formId(parentPluginName, masters, 0x02000001);
      EXPECT_EQ(parentPluginName, formId.getPluginName());
    }

    TEST_F(FormIdTest, formIdsWithEqualObjectIndicesButDifferentModIndiciesShouldNotBeEqual) {
      FormId formId(parentPluginName, masters, 0x01);
      FormId otherFormId(parentPluginName, masters, 0x05000001);

      EXPECT_NE(formId, otherFormId);
    }

    TEST_F(FormIdTest, formIdsWithEqualModIndicesButDifferentObjectIndiciesShouldNotBeEqual) {
      FormId formId(parentPluginName, masters, 0x05000001);
      FormId otherFormId(parentPluginName, masters, 0x05000002);

      EXPECT_NE(formId, otherFormId);
    }

    TEST_F(FormIdTest, formIdsWithEqualModIndicesAndNoMastersAndDifferentParentPluginNamesShouldNotBeEqual) {
        FormId formId(parentPluginName, std::vector<std::string>(), 0x01);
        FormId otherFormId(parentPluginName + ".ghost", std::vector<std::string>(), 0x01);

        EXPECT_NE(formId, otherFormId);
    }

    TEST_F(FormIdTest, identicalFormIdsShouldBeEqual) {
      FormId formId(parentPluginName, masters, 0x01);
      FormId otherFormId(parentPluginName, masters, 0x01);

      EXPECT_EQ(formId, otherFormId);
    }

    TEST_F(FormIdTest, formIdWithLowerObjectIndexShouldBeLessThanAnotherWithTheSamePlugin) {
      FormId formId(parentPluginName, masters, 0x01);
      FormId otherFormId(parentPluginName, masters, 0x02);

      EXPECT_LT(formId, otherFormId);
      EXPECT_FALSE(otherFormId < formId);
    }

    TEST_F(FormIdTest, formIdWithCaseInsensitiveLexicographicallyLowerPluginShouldBeLessThanAnotherWithTheSameObjectIndex) {
      FormId formId("A", std::vector<std::string>(), 0x01);
      FormId otherFormId("b", std::vector<std::string>(), 0x01);

      EXPECT_LT(formId, otherFormId);
      EXPECT_FALSE(otherFormId < formId);
    }

    TEST_F(FormIdTest, identicalFormIdsShouldNotBeLessThanOneAnother) {
      FormId formId(parentPluginName, masters, 0x01);
      FormId otherFormId(parentPluginName, masters, 0x01);

      EXPECT_FALSE(formId < otherFormId);
      EXPECT_FALSE(otherFormId < formId);
    }
  }
}
