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

#include <string>
#include <vector>
#include <stdint.h>

#include <yaml-cpp/yaml.h>

namespace espm {

    struct Settings {
        Settings() : progressCallback(NULL), progressObject(NULL) {}
        Settings(const std::string& filepath, const std::string& game) : _game(game), progressCallback(NULL), progressObject(NULL) {

            YAML::Node contents = YAML::LoadFile(filepath);  //Throws exception on failure.

            YAML::Node ys = contents[game];

            group.type = ys["Group Type"].as<std::string>();

            if (ys["Group Lengths"]) {
                group.type_len      = ys["Group Lengths"]["Type"].as<unsigned int>();
                group.size_len      = ys["Group Lengths"]["Size"].as<unsigned int>();
                group.label_len     = ys["Group Lengths"]["Label"].as<unsigned int>();
                group.groupType_len = ys["Group Lengths"]["GroupType"].as<unsigned int>();
                group.stamp_len     = ys["Group Lengths"]["Stamp"].as<unsigned int>();
                group.unk1_len       = ys["Group Lengths"]["Unknown 1"].as<unsigned int>();
                group.ver_len       = ys["Group Lengths"]["Version"].as<unsigned int>();
                group.unk2_len       = ys["Group Lengths"]["Unknown 2"].as<unsigned int>();
            }

            record.comp_flag = ys["Compression Flag"].as<uint32_t>();
            record.mast_flag = ys["Master Flag"].as<uint32_t>();

            record.type_len  = ys["Record Lengths"]["Type"].as<unsigned int>();
            record.size_len  = ys["Record Lengths"]["Size"].as<unsigned int>();
            record.unk1_len   = ys["Record Lengths"]["Unknown 1"].as<unsigned int>();
            record.flags_len = ys["Record Lengths"]["Flags"].as<unsigned int>();
            record.id_len    = ys["Record Lengths"]["ID"].as<unsigned int>();
            record.rev_len   = ys["Record Lengths"]["Revision"].as<unsigned int>();
            record.ver_len   = ys["Record Lengths"]["Version"].as<unsigned int>();
            record.unk2_len   = ys["Record Lengths"]["Unknown 2"].as<unsigned int>();

            field.type_len = ys["Field Lengths"]["Type"].as<unsigned int>();
            field.size_len = ys["Field Lengths"]["Size"].as<unsigned int>();

        }

        std::string _game;
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

    inline void InitPredefinedSettings(const std::string game, Settings& settings) {

        if (game == "Morrowind") {
            /* Init Morrowind settings. */

            settings.group.type = "";
            settings.group.type_len = 0;
            settings.group.size_len = 0;
            settings.group.label_len = 0;
            settings.group.groupType_len = 0;
            settings.group.stamp_len = 0;
            settings.group.unk1_len = 0;
            settings.group.ver_len = 0;
            settings.group.unk2_len = 0;

            settings.record.comp_flag = 0;
            settings.record.mast_flag = 0;
            settings.record.type_len = 4;
            settings.record.size_len = 4;
            settings.record.unk1_len = 4;
            settings.record.flags_len = 4;
            settings.record.id_len = 0;
            settings.record.rev_len = 0;
            settings.record.ver_len = 0;
            settings.record.unk2_len = 0;

            settings.field.type_len = 4;
            settings.field.size_len = 4;
        } else if (game == "Oblivion") {
            /* Init Oblivion settings. */

            settings.group.type = "GRUP";
            settings.group.type_len = 4;
            settings.group.size_len = 4;
            settings.group.label_len = 4;
            settings.group.groupType_len = 4;
            settings.group.stamp_len = 4;
            settings.group.unk1_len = 0;
            settings.group.ver_len = 0;
            settings.group.unk2_len = 0;

            settings.record.comp_flag = 0x00040000;
            settings.record.mast_flag = 0x00000001;
            settings.record.type_len = 4;
            settings.record.size_len = 4;
            settings.record.unk1_len = 0;
            settings.record.flags_len = 4;
            settings.record.id_len = 4;
            settings.record.rev_len = 4;
            settings.record.ver_len = 0;
            settings.record.unk2_len = 0;

            settings.field.type_len = 4;
            settings.field.size_len = 2;
        } else if (game == "Skyrim" || game == "Fallout3" || game == "FalloutNV") {
            /* Init Skyrim settings. */

            settings.group.type = "GRUP";
            settings.group.type_len = 4;
            settings.group.size_len = 4;
            settings.group.label_len = 4;
            settings.group.groupType_len = 4;
            settings.group.stamp_len = 2;
            settings.group.unk1_len = 2;
            settings.group.ver_len = 2;
            settings.group.unk2_len = 2;

            settings.record.comp_flag = 0x00040000;
            settings.record.mast_flag = 0x00000001;
            settings.record.type_len = 4;
            settings.record.size_len = 4;
            settings.record.unk1_len = 0;
            settings.record.flags_len = 4;
            settings.record.id_len = 4;
            settings.record.rev_len = 4;
            settings.record.ver_len = 2;
            settings.record.unk2_len = 2;

            settings.field.type_len = 4;
            settings.field.size_len = 2;
        }
    }
}

#endif
