/*
 * playground.h
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
#ifndef __ESPM_FILES__
#define __ESPM_FILES__

#include "settings.h"
#include "record.h"
#include "group.h"
#include "streams.h"

#include <string>
#include <vector>
#include <stdint.h>

#include <zlib.h>
#include <boost/crc.hpp>

namespace espm {

    //Abstract base class.
    struct File {
        std::vector<Group> groups;
        std::vector<Record> records;
        uint32_t crc;

        File() {}

        // If headerOnly is true, a side effect is that the CRC is not calculated.
        File(const boost::filesystem::path filepath, const Settings& settings, bool readFields, bool headerOnly) : crc(0) {

            ifstream input(filepath, std::ios::binary);
            input.exceptions(std::ios_base::badbit);

            if (headerOnly) {
                Record header;
                header.read(input, settings, true);
                records.push_back(header);

                input.close();
                return;
            }

            input.seekg(0, input.end);
            unsigned int length = input.tellg();
            input.seekg(0, input.beg);

            //Allocate memory for file contents.
            char * buffer = new char[length];

            //Read whole file in.
            input.read(buffer, length);

            input.close();

            //Calculate the file CRC.
            boost::crc_32_type result;
            result.process_bytes(buffer, length);
            crc = result.checksum();

            //First read the TES4/TES3 header.
            Record header;
            uint32_t count = header.read(buffer, settings, true);
            records.push_back(header);

            if (!headerOnly) {
                if (settings.group.type.empty()) {  //Morrowind.
                    while (count < length) {
                        Record record;
                        count += record.read(buffer + count, settings, readFields);
                        records.push_back(record);
                    }
                } else {
                    while (count < length) {
                        Group group;
                        count += group.read(buffer + count, settings, readFields);
                        groups.push_back(group);
                    }
                }
            }

            delete [] buffer;
        }

        virtual bool isMaster(const Settings& settings) const = 0;

        virtual std::vector<std::string> getMasters() const = 0;

        virtual std::string getDescription() const = 0;

        std::vector<uint32_t> getFormIDs() {
            std::vector<uint32_t> formids;
            for (int i=0,max=groups.size(); i < max; ++i) {
                std::vector<uint32_t> fids = groups[i].getFormIDs();
                formids.insert(formids.end(), fids.begin(), fids.end());
            }
            for (int i=1,max=records.size(); i < max; ++i) {  //Skip the first record, since it has a FormID of zero (it's the TES4 record).
                formids.push_back(records[i].id);
            }
            return formids;
        }

        bool getRecordByFieldData(char * type, char * data, uint32_t dataSize, Record& record, const Settings& settings) const {
            for (int i=0,max=groups.size(); i < max; ++i) {
                if (groups[i].getRecordByFieldData(type, data, dataSize, record, settings))
                    return true;
            }
            for (std::vector<Record>::const_iterator it=records.begin(),endIt=records.end(); it != endIt; ++it) {
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
            for (int i=0,max=groups.size(); i < max; ++i) {
                std::vector<Record> g_recs = groups[i].getRecords();
                recs.insert(recs.end(), g_recs.begin(), g_recs.end());
            }
            return records;
        }

        bool getRecordByID(uint32_t id, Record& record) const {
            for (int i=0,max=groups.size(); i < max; ++i) {
                if (groups[i].getRecordByID(id, record))
                    return true;
            }
            for (std::vector<Record>::const_iterator it=records.begin(),endIt=records.end(); it != endIt; ++it) {
                if (it->id == id) {
                    record = *it;
                    return true;
                }
            }
            return false;
        }

        bool getGroupByType(char * type, Group& group, const Settings& settings) const {
            for (int i=0,max=groups.size(); i < max; ++i) {
                if (strncmp(type, groups[i].type, settings.group.type_len) == 0) {
                    group = groups[i];
                    return true;
                }
            }
            return false;
        }

        std::vector<Group> getGroups() const {
            return groups;
        }
    };

}

#endif
