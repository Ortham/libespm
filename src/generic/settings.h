/*
 * settings.h
 * This file is part of libespm
 *
 * Copyright (C) 2013 WrinklyNinja
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

#ifndef __ESPM_SETTINGS__
#define __ESPM_SETTINGS__

#include "streams.h"

#include <string>
#include <vector>
#include <cstdint>

#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

namespace espm {

    struct Settings {
        Settings() : progressCallback(nullptr), progressObject(nullptr) {}
        Settings(const boost::filesystem::path& filepath, const std::string& game) : progressCallback(nullptr), progressObject(nullptr) {

            if (!boost::filesystem::exists(filepath))
                return;

            try {
                espm::ifstream input(filepath, std::ios::binary);
                input.exceptions(std::ios_base::badbit);

                std::string line;
                while (getline(input, line)) {
                    if (!boost::starts_with(line, game))
                        continue;

                    size_t pos1 = line.find('.');
                    size_t pos2 = line.find('=');
                    if (pos1 == std::string::npos || pos2 == std::string::npos)
                        continue;

                    std::string key = boost::trim_copy(line.substr(pos1 + 1, pos2 - pos1 - 1));
                    std::string value = boost::trim_copy(line.substr(pos2 + 1));

                    if (key == "group.type") {
                       group.type = value;
                    } else if (key == "group.type_len") {
                       group.type_len = std::stoul(value);
                    } else if (key == "group.size_len") {
                       group.size_len = std::stoul(value);
                    } else if (key == "group.label_len") {
                       group.label_len = std::stoul(value);
                    } else if (key == "group.groupType_len") {
                       group.groupType_len = std::stoul(value);
                    } else if (key == "group.stamp_len") {
                       group.stamp_len = std::stoul(value);
                    } else if (key == "group.unk1_len") {
                       group.unk1_len = std::stoul(value);
                    } else if (key == "group.ver_len") {
                       group.ver_len = std::stoul(value);
                    } else if (key == "group.unk2_len") {
                       group.unk2_len = std::stoul(value);
                    } else if (key == "record.comp_flag") {
                        record.comp_flag = std::stoul(value, nullptr, 16);
                    } else if (key == "record.mast_flag") {
                        record.mast_flag = std::stoul(value, nullptr, 16);
                    } else if (key == "record.type_len") {
                       record.type_len = std::stoul(value);
                    } else if (key == "record.size_len") {
                       record.size_len = std::stoul(value);
                    } else if (key == "record.unk1_len") {
                       record.unk1_len = std::stoul(value);
                    } else if (key == "record.flags_len") {
                       record.flags_len = std::stoul(value);
                    } else if (key == "record.id_len") {
                       record.id_len = std::stoul(value);
                    } else if (key == "record.rev_len") {
                       record.rev_len = std::stoul(value);
                    } else if (key == "record.ver_len") {
                       record.ver_len = std::stoul(value);
                    } else if (key == "record.unk2_len") {
                       record.unk2_len = std::stoul(value);
                    } else if (key == "field.type_len") {
                       field.type_len = std::stoul(value);
                    } else if (key == "field.size_len") {
                       field.size_len = std::stoul(value);
                    }
                }

                input.close();
            } catch (std::exception& /*e*/) {
            }
        }
        Settings(const std::string& game) : progressCallback(nullptr), progressObject(nullptr) {
            if (game == "tes3") {
                /* Init Morrowind settings. */

                group.type = "";
                group.type_len = 0;
                group.size_len = 0;
                group.label_len = 0;
                group.groupType_len = 0;
                group.stamp_len = 0;
                group.unk1_len = 0;
                group.ver_len = 0;
                group.unk2_len = 0;

                record.comp_flag = 0;
                record.mast_flag = 0;
                record.type_len = 4;
                record.size_len = 4;
                record.unk1_len = 4;
                record.flags_len = 4;
                record.id_len = 0;
                record.rev_len = 0;
                record.ver_len = 0;
                record.unk2_len = 0;

                field.type_len = 4;
                field.size_len = 4;
            } else if (game == "tes4") {
                /* Init Oblivion settings. */

                group.type = "GRUP";
                group.type_len = 4;
                group.size_len = 4;
                group.label_len = 4;
                group.groupType_len = 4;
                group.stamp_len = 4;
                group.unk1_len = 0;
                group.ver_len = 0;
                group.unk2_len = 0;

                record.comp_flag = 0x00040000;
                record.mast_flag = 0x00000001;
                record.type_len = 4;
                record.size_len = 4;
                record.unk1_len = 0;
                record.flags_len = 4;
                record.id_len = 4;
                record.rev_len = 4;
                record.ver_len = 0;
                record.unk2_len = 0;

                field.type_len = 4;
                field.size_len = 2;
            } else if (game == "tes5" || game == "fo3" || game == "fonv") {
                /* Init Skyrim settings. */

                group.type = "GRUP";
                group.type_len = 4;
                group.size_len = 4;
                group.label_len = 4;
                group.groupType_len = 4;
                group.stamp_len = 2;
                group.unk1_len = 2;
                group.ver_len = 2;
                group.unk2_len = 2;

                record.comp_flag = 0x00040000;
                record.mast_flag = 0x00000001;
                record.type_len = 4;
                record.size_len = 4;
                record.unk1_len = 0;
                record.flags_len = 4;
                record.id_len = 4;
                record.rev_len = 4;
                record.ver_len = 2;
                record.unk2_len = 2;

                field.type_len = 4;
                field.size_len = 2;
            }
        }
        void (*progressCallback)(void * pointer);
        void * progressObject;

        struct {
            std::string type;

            unsigned int type_len;
            unsigned int size_len;
            unsigned int label_len;
            unsigned int groupType_len;
            unsigned int stamp_len;
            unsigned int unk1_len;
            unsigned int ver_len;
            unsigned int unk2_len;
        } group;

        struct {
            uint32_t comp_flag;
            uint32_t mast_flag;

            unsigned int type_len;
            unsigned int size_len;
            unsigned int unk1_len;
            unsigned int flags_len;
            unsigned int id_len;
            unsigned int rev_len;
            unsigned int ver_len;
            unsigned int unk2_len;
        } record;

        struct {
            unsigned int type_len;
            unsigned int size_len;
        } field;
    };
}

#endif
