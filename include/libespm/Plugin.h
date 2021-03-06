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
#include <istream>
#include <vector>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/locale/encoding.hpp>
#include <boost/iostreams/device/mapped_file.hpp>
#include <boost/iostreams/stream.hpp>

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

      // Memory-map the file, this can save memory and significantly improve
      // performance when reading large plugins.
      boost::iostreams::mapped_file_source mmap(filepath);
      boost::iostreams::stream<boost::iostreams::mapped_file_source> input(mmap, std::ios::binary);
      input.exceptions(std::ios_base::badbit | std::ios_base::failbit);

      headerRecord.read(input, gameId, false);
      if ((gameId == GameId::MORROWIND && headerRecord.getType() != "TES3")
          || (gameId != GameId::MORROWIND && headerRecord.getType() != "TES4"))
          throw std::runtime_error(name + " is not a valid plugin.");

      if (loadHeaderOnly)
        return;

      // If the filename ends in ".ghost", trim that extension.
      std::string trimmedName = trimToEspm(name);

      std::vector<std::string> masters = getMasters();
      uintmax_t fileSize = boost::filesystem::file_size(filepath);
      if (gameId == GameId::MORROWIND) {
        while (input.good() && (uintmax_t)input.tellg() < fileSize) {
          Record record;
          record.read(input, gameId, false);
          formIds.insert(FormId(trimmedName, masters, record.getFormId()));
        }
      }
      else {
        while (input.good() && (uintmax_t)input.tellg() < fileSize) {
          Group group;
          group.read(input, gameId, true);
          for (const auto& formId : group.getRecordFormIds()) {
            formIds.insert(FormId(trimmedName, masters, formId));
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
        return (headerRecord.getFlags() & 0x00000001) != 0;
    }

    inline static bool isValid(const boost::filesystem::path& filepath, GameId gameId, bool loadHeaderOnly = false) {
      Plugin plugin(gameId);
      try {
        plugin.load(filepath, loadHeaderOnly);
        return true;
      }
      catch (std::exception&) {
        return false;
      }
    }

    inline std::vector<std::string> getMasters() const {
      std::vector<std::string> masterFilenames;
      for (const auto& subrecord : headerRecord.getSubrecords()) {
        if (subrecord.getType() == "MAST") {
          auto rawData = subrecord.getRawData();
          std::string masterFilename(rawData.first.get());
          masterFilenames.push_back(convertToUtf8(masterFilename));
        }
      }
      return masterFilenames;
    }

    inline std::string getDescription() const {
      std::string targetSubrecordType("SNAM");
      ptrdiff_t descriptionOffset(0);
      if (gameId == GameId::MORROWIND) {
        targetSubrecordType = "HEDR";
        descriptionOffset = 40;
      }

      for (const auto& subrecord : headerRecord.getSubrecords()) {
        if (subrecord.getType() == targetSubrecordType) {
          auto rawData = subrecord.getRawData();
          return convertToUtf8(std::string(rawData.first.get() + descriptionOffset));
        }
      }
      return "";
    }

    inline std::set<FormId> getFormIds() const {
      if (gameId == GameId::MORROWIND)
        throw std::domain_error("Cannot get FormIDs for a Morrowind plugin: Morrowind does not use FormIDs.");

      return formIds;
    }

    inline uint32_t getRecordAndGroupCount() const {
      ptrdiff_t countOffset(4);
      if (gameId == GameId::MORROWIND)
        countOffset = 296;

      for (const auto& subrecord : headerRecord.getSubrecords()) {
        if (subrecord.getType() == "HEDR") {
          auto rawData = subrecord.getRawData();
          return *reinterpret_cast<uint32_t*>(rawData.first.get() + countOffset);
        }
      }
      return 0;
    }

  private:
    inline static std::string trimToEspm(const std::string& filename) {
      size_t pos = filename.find(".esp");
      if (pos != std::string::npos && pos != filename.length() - 4)
        return filename.substr(0, pos + 4);

      pos = filename.find(".esm");
      if (pos != std::string::npos && pos != filename.length() - 4)
        return filename.substr(0, pos + 4);

      return filename;
    }

    inline static std::string convertToUtf8(const std::string& windows1252String) {
      return boost::locale::conv::to_utf<char>(windows1252String, "Windows-1252", boost::locale::conv::stop);
    }
  };
}

#endif
