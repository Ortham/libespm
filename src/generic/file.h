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
#include <cstdint>

#include <boost/crc.hpp>

namespace espm {
    //Abstract base class.
    struct File {
        std::vector<Group> groups;
        std::vector<Record> records;
        uint32_t crc;

        File() : crc(0) {}

        // If headerOnly is true, a side effect is that the CRC is not calculated.
        // Can throw exceptions.
        File(const boost::filesystem::path& filepath, const Settings& settings, bool readFields, bool headerOnly) : crc(0) {
            if (!boost::filesystem::exists(filepath))
                throw std::runtime_error("File doesn't exist.");
            else if (boost::filesystem::file_size(filepath) == 0)
                throw std::runtime_error("File is empty.");

            ifstream input(filepath, std::ios::binary);
            input.exceptions(std::ios_base::badbit | std::ios_base::failbit);

            if (headerOnly) {
                Record header;
                header.read(input, settings, true);
                records.push_back(header);

                input.close();
                return;
            }

            input.seekg(0, input.end);
            std::streamoff offset = input.tellg();
            if (offset < 0)
                throw std::runtime_error("File stream offset is negative.");
            size_t length = size_t(offset);
            input.seekg(0, input.beg);

            //Allocate memory for file contents.
            char * buffer;
            buffer = new char[length];

            //Read whole file in.
            input.read(buffer, length);
            input.close();

            //Calculate the file CRC.
            boost::crc_32_type result;
            result.process_bytes(buffer, length);
            crc = result.checksum();

            //First read the TES4/TES3 header.
            Record header;
            uint32_t count = header.read(buffer, length, settings, true);
            records.push_back(header);

            if (!headerOnly) {
                if (settings.group.type.empty()) {  //Morrowind.
                    while (count < length) {
                        Record record;
                        count += record.read(buffer + count, length - count, settings, readFields);
                        records.push_back(record);
                    }
                }
                else {
                    while (count < length) {
                        Group group;
                        count += group.read(buffer + count, length - count, settings, readFields);
                        groups.push_back(group);
                    }
                }
            }

            delete[] buffer;
        }

        virtual bool isMaster(const Settings& settings) const = 0;

        virtual std::vector<std::string> getMasters() const = 0;

        virtual std::string getDescription() const = 0;

        std::vector<uint32_t> getFormIDs() {
            std::vector<uint32_t> formids;
            for (const auto &group : groups) {
                std::vector<uint32_t> fids(group.getFormIDs());
                formids.insert(formids.end(), fids.begin(), fids.end());
            }
            for (size_t i = 1, max = records.size(); i < max; ++i) {  //Skip the first record, since it has a FormID of zero (it's the TES4 record).
                formids.push_back(records[i].id);
            }
            return formids;
        }

        bool getRecordByFieldData(char * type, char * data, uint32_t dataSize, Record& outRecord, const Settings& settings) const {
            for (const auto &group : groups) {
                if (group.getRecordByFieldData(type, data, dataSize, outRecord, settings))
                    return true;
            }
            for (const auto &record : records) {
                for (const auto &field : record.fields) {
                    if (field.dataSize == dataSize
                        && strncmp(field.type, type, settings.group.type_len) == 0
                        && memcmp(field.data, data, dataSize) == 0) {
                        outRecord = record;
                        return true;
                    }
                }
            }
            return false;
        }

        std::vector<Record> getRecords() const {
            std::vector<Record> recs(records);
            for (const auto &group : groups) {
                std::vector<Record> g_recs(group.getRecords());
                recs.insert(recs.end(), g_recs.begin(), g_recs.end());
            }
            return records;
        }

        bool getRecordByID(uint32_t id, Record& outRecord) const {
            for (const auto &group : groups) {
                if (group.getRecordByID(id, outRecord))
                    return true;
            }
            for (const auto &record : records) {
                if (record.id == id) {
                    outRecord = record;
                    return true;
                }
            }
            return false;
        }

        bool getGroupByType(char * type, Group& outGroup, const Settings& settings) const {
            for (const auto &group : groups) {
                if (strncmp(type, group.type, settings.group.type_len) == 0) {
                    outGroup = group;
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
