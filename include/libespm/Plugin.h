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

#ifndef LIBESPM_PLUGIN
#define LIBESPM_PLUGIN

#include <string>
#include <fstream>
#include <vector>
#include <sstream>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include "FormId.h"
#include "Group.h"

namespace libespm {
  class Plugin {
  private:
    GameId gameId;

    std::string name;
    Record headerRecord;
    std::set<FormId> formIds;

  public:
    inline Plugin(GameId gameId) : gameId(gameId) {}

    inline void load(const boost::filesystem::path& filepath, bool loadHeaderOnly = false) {
      name = filepath.filename().string();

      boost::filesystem::ifstream input(filepath, std::ios::binary);
      input.exceptions(std::ios_base::badbit | std::ios_base::failbit);

      if (loadHeaderOnly) {
        headerRecord.read(input, gameId, false);
        return;
      }

      // Read the whole file in at once.
      std::stringstream bufferStream;
      bufferStream.exceptions(std::ios_base::badbit | std::ios_base::failbit);
      bufferStream << input.rdbuf();

      headerRecord.read(bufferStream, gameId, false);

      std::vector<std::string> masters = getMasters();
      uintmax_t fileSize = boost::filesystem::file_size(filepath);
      if (gameId == GameId::MORROWIND) {
        while (bufferStream.good() && bufferStream.tellg() < fileSize) {
          Record record;
          record.read(bufferStream, gameId, false);
          formIds.insert(FormId(name, masters, record.getFormId()));
        }
      }
      else {
        while (bufferStream.good() && bufferStream.tellg() < fileSize) {
          Group group;
          group.read(bufferStream, gameId, true);
          std::set<uint32_t> groupRecordFormIds = group.getRecordFormIds();
          for (auto formId : groupRecordFormIds) {
            formIds.insert(FormId(name, masters, formId));
          }
        }
      }
    }

    inline std::string getName() const {
      return name;
    }

    inline bool isMasterFile() const {
      if (gameId == GameId::MORROWIND) {
        if (name.length() < 5)
          return false;

        const std::string masterFileExtension = ".esm";
        std::string fileExtension = name.substr(name.length() - 4);

        return std::equal(begin(fileExtension),
                          end(fileExtension),
                          begin(masterFileExtension),
                          [](char lhs, char rhs) {
          return tolower(lhs) == tolower(rhs);
        });
      }
      else
        return headerRecord.isMasterFlagSet();
    }

    inline static bool isValid(const boost::filesystem::path& filepath, GameId gameId) {
      Plugin plugin(gameId);
      try {
        plugin.load(filepath);
        return true;
      }
      catch (std::exception&) {
        return false;
      }
    }

    inline std::vector<std::string> getMasters() const {
      return headerRecord.getMasters();
    }

    inline std::string getDescription() const {
      return headerRecord.getDescription();
    }

    inline std::set<FormId> getFormIds() const {
      if (gameId == GameId::MORROWIND)
        throw std::domain_error("Cannot get FormIDs for a Morrowind plugin: Morrowind does not use FormIDs.");

      return formIds;
    }
  };
}

#endif
