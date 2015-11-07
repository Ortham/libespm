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

#ifndef LIBESPM2_PLUGIN
#define LIBESPM2_PLUGIN

#include <string>
#include <fstream>
#include <vector>

#include <boost/filesystem.hpp>

#include "FormId.h"
#include "Group.h"
#include "Record.h"

namespace libespm2 {
  class Plugin {
  private:
    std::string name;
    Record tes4Record;
    std::set<FormId> _formIds;

  public:
    inline void load(const boost::filesystem::path& filepath) {
      std::ifstream input(filepath.string(), std::ios::binary);
      if (!input.good())
        throw std::runtime_error("Cannot open plugin file at " + filepath.string());

      name = filepath.filename().string();

      tes4Record.read(input);

      std::vector<std::string> masters = getMasters();
      size_t fileSize = boost::filesystem::file_size(filepath);
      while (input.tellg() < fileSize) {
        Group group;
        group.read(input);
        std::set<uint32_t> groupRecordFormIds = group.getRecordFormIds();
        for (auto formId : groupRecordFormIds) {
          _formIds.insert(FormId(name, masters, formId));
        }
      }

      input.close();
    }

    inline std::string getName() const {
      return name;
    }

    inline bool isMasterFile() const {
      return tes4Record.isMasterFlagSet();
    }

    inline static bool isValid(const boost::filesystem::path& filepath) {
      Plugin plugin;
      try {
        plugin.load(filepath);
        return true;
      }
      catch (std::exception&) {
        return false;
      }
    }

    inline std::vector<std::string> getMasters() const {
      return tes4Record.getMasters();
    }

    inline std::string getDescription() const {
      return tes4Record.getDescription();
    }

    inline std::set<FormId> getFormIds() const {
      return _formIds;
    }
  };
}

#endif
