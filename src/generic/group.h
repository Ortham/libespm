/*
 * groups.h
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

#ifndef __ESPM_GROUPS__
#define __ESPM_GROUPS__

#include "settings.h"
#include "record.h"

#include <string>
#include <stdint.h>

namespace espm {

    struct Group {
        char type[4];
        char label[4];

        uint32_t groupSize;
        uint32_t groupType;

        uint16_t stamp;
        uint16_t unknown1;
        uint16_t version;
        uint16_t unknown2;

        std::vector<Record> records;
        std::vector<Group> subgroups;

        uint32_t readHeader(char * buffer, const Settings& settings) {
            uint32_t headerSize =
                settings.group.type_len +
                settings.group.size_len +
                settings.group.label_len +
                settings.group.groupType_len +
                settings.group.stamp_len +
                settings.group.unk1_len +
                settings.group.ver_len +
                settings.group.unk2_len;

            memcpy(type, buffer, settings.group.type_len);
            buffer += settings.group.type_len;

            groupSize = *reinterpret_cast<uint32_t*>(buffer);
            buffer += settings.group.size_len;

            memcpy(label, buffer, settings.group.label_len);
            buffer += settings.group.label_len;

            groupType = *reinterpret_cast<uint32_t*>(buffer);
            buffer += settings.group.groupType_len;

            stamp = *reinterpret_cast<uint16_t*>(buffer);
            buffer += settings.group.stamp_len;

            unknown1 = *reinterpret_cast<uint16_t*>(buffer);
            buffer += settings.group.unk1_len;

            version = *reinterpret_cast<uint16_t*>(buffer);
            buffer += settings.group.ver_len;

            unknown2 = *reinterpret_cast<uint16_t*>(buffer);
            buffer += settings.group.unk2_len;
            buffer -= headerSize;

            return headerSize;
        }

        uint32_t read(char * buffer, const Settings& settings, bool readFields) {

            uint32_t count = readHeader(buffer, settings);

            while (count < groupSize) {
                char temp[4];
                memcpy(temp, buffer + count, sizeof(temp));

                if (strncmp(temp, settings.group.type.data(), settings.group.type_len) == 0) {
                    Group subgroup;
                    count += subgroup.read(buffer + count, settings, readFields);
                    subgroups.push_back(subgroup);
                } else {
                    Record record;
                    count += record.read(buffer + count, settings, readFields);
                    records.push_back(record);
                }
            }

            return count;
        }

        std::vector<uint32_t> getFormIDs() {
            std::vector<uint32_t> formids;
            for (int i=0,max=subgroups.size(); i < max; ++i) {
                std::vector<uint32_t> fids = subgroups[i].getFormIDs();
                formids.insert(formids.end(), fids.begin(), fids.end());
            }
            for (int i=0,max=records.size(); i < max; ++i) {
                formids.push_back(records[i].id);
            }
            return formids;
        }

        bool getRecordByFieldData(char * type, char * data, uint32_t dataSize, Record& record, const Settings& settings) const {
            std::vector<Record> recs = getRecords();
            for (std::vector<Record>::const_iterator it=recs.begin(),endIt=recs.end(); it != endIt; ++it) {
                for (std::vector<Field>::const_iterator jt=it->fields.begin(), endjt=it->fields.end(); jt != endjt; ++jt) {
                    if (jt->dataSize == dataSize
                     && strncmp(jt->type, type, settings.group.type_len) == 0
                     && memcmp(jt->data, data, dataSize) == 0) {
                        record = *it;
                        return true;
                    }
                }
            }
            return false;
        }

        std::vector<Record> getRecords() const {
            std::vector<Record> recs = records;
            for (std::vector<Group>::const_iterator it=subgroups.begin(),endIt=subgroups.end(); it != endIt; ++it) {
                std::vector<Record> g_recs = it->getRecords();
                recs.insert(recs.end(), g_recs.begin(), g_recs.end());
            }
            return records;
        }

        bool getRecordByID(uint32_t id, Record& record) const {
            std::vector<Record> recs = getRecords();
            for (std::vector<Record>::const_iterator it=recs.begin(),endIt=recs.end(); it != endIt; ++it) {
                if (it->id == id) {
                    record = *it;
                    return true;
                }
            }
            return false;
        }
    };

}

#endif
