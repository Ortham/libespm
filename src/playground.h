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
#ifndef __PLAYGROUND__
#define __PLAYGROUND__

#include <string>
#include <vector>
#include <stdint.h>

#include <yaml-cpp/yaml.h>
#include <zlib.h>

#include <boost/filesystem.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/file_descriptor.hpp>

/*
    Libespm playground for BOSSv3.
*/

namespace espm {

    typedef boost::iostreams::stream< boost::iostreams::file_descriptor_sink > ofstream;
    typedef boost::iostreams::stream< boost::iostreams::file_descriptor_source > ifstream;
    typedef boost::iostreams::stream< boost::iostreams::file_descriptor > fstream;

    struct Settings {
        Settings() {}
        Settings(const std::string& filepath, const std::string& game) {

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

            /*if (ys["BSAHead"])
                bsa_head  = ys["BSAHead"].as<std::string>();

            if (ys["PlugHead"])
                plug_head = ys["PlugHead"].as<std::string>();

            if (ys["SaveHead"])
                save_head = ys["SaveHead"].as<std::string>();

            if (ys["MastExt"])
                mast_ext  = ys["MastExt"].as<std::string>();

            if (ys["PlugExt"])
                plug_ext  = ys["PlugExt"].as<std::string>();

            if (ys["SaveExt"])
                save_ext  = ys["SaveExt"].as<std::string>();

            if (ys["ONAM"])
                onam = ys["ONAM"].as< std::vector<std::string> >();*/
        }

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

        // Unused by these parsers.
        /*std::string bsa_head;
        std::string plug_head;
        std::string save_head;
        std::string mast_ext;
        std::string plug_ext;
        std::string save_ext;
        std::vector<std::string> onam;*/
    };

    struct Field {
        Field() : data(NULL), dataSize(0) {}
        Field(const Field& field) {
            dataSize = field.dataSize;
            data = new char[dataSize];
            memcpy(type, field.type, 4);
            memcpy(data, field.data, dataSize);
        }

        ~Field() {
            delete [] data;
        }

        Field& operator = (const Field& rhs) {
            if (this != &rhs) {
                delete [] data;

                dataSize = rhs.dataSize;
                data = new char[dataSize];
                memcpy(type, rhs.type, 4);
                memcpy(data, rhs.data, dataSize);
            }

            return *this;
        }

        char type[4];
        uint32_t dataSize;  //Usually 16 bit, but 32 bit needed for XXXX-extended subrecords.
        char * data;

        uint32_t read(char * buffer, const Settings& settings) {

            memcpy(type, buffer, settings.field.type_len);
            memcpy(&dataSize, buffer + settings.field.type_len, settings.field.size_len);

            data = new char[dataSize];
            memcpy(data, buffer + settings.field.type_len + settings.field.size_len, dataSize);

            return (uint32_t)settings.field.type_len + settings.field.size_len + dataSize;
        }

        uint32_t read(char * buffer, const Settings& settings, const Field& lastField) {

            memcpy(type, buffer, settings.field.type_len);

            if (strncmp(lastField.type,"XXXX", 4) == 0)
                memcpy(&dataSize, lastField.data, 4);
            else
                memcpy(&dataSize, buffer + settings.field.type_len, settings.field.size_len);

            data = new char[dataSize];
            memcpy(data, buffer + settings.field.type_len + settings.field.size_len, dataSize);


            return (uint32_t)settings.field.type_len + settings.field.size_len + dataSize;
        }
    };

    struct Record {
        char type[4];

        uint32_t dataSize;
        uint16_t unknown1;
        uint32_t flags;
        uint32_t id;
        uint32_t revision;

        uint16_t version;
        uint16_t unknown2;

        std::vector<Field> fields;

        bool isCompressed(const Settings& settings) const {
            return flags & settings.record.comp_flag;
        }

        uint32_t readHeader(char * buffer, const Settings& settings) {
            uint32_t headerSize =
                settings.record.type_len +
                settings.record.size_len +
                settings.record.unk1_len +
                settings.record.flags_len +
                settings.record.id_len +
                settings.record.rev_len +
                settings.record.ver_len +
                settings.record.unk2_len;

            memcpy(type, buffer, settings.record.type_len);
            buffer += settings.record.type_len;

            dataSize = *reinterpret_cast<uint32_t*>(buffer);
            buffer += settings.record.size_len;

            unknown1 = *reinterpret_cast<uint32_t*>(buffer);
            buffer += settings.record.unk1_len;

            flags = *reinterpret_cast<uint32_t*>(buffer);
            buffer += settings.record.flags_len;

            id = *reinterpret_cast<uint32_t*>(buffer);
            buffer += settings.record.id_len;

            revision = *reinterpret_cast<uint32_t*>(buffer);
            buffer += settings.record.rev_len;

            version = *reinterpret_cast<uint16_t*>(buffer);
            buffer += settings.record.ver_len;

            unknown2 = *reinterpret_cast<uint16_t*>(buffer);
            buffer += settings.record.unk2_len;
            buffer -= headerSize;

            return headerSize;
        }

        uint32_t readFields(char * buffer, const Settings& settings) {

            char * trueData = buffer;
            uint32_t trueDataSize = dataSize;
            if (isCompressed(settings)) {
                //Need to uncompress the data.
                uint32_t compSize = dataSize - 4;
                uint32_t trueDataSize = *reinterpret_cast<uint32_t*>(trueData);
                trueData += 4;

                char * decompData = new char[trueDataSize];

                int ret = uncompress((Bytef*)decompData, (uLongf*)&trueDataSize, (Bytef*)trueData, (uLong)compSize);

                trueData = decompData;
            }

            uint32_t count = 0;
            while (count < trueDataSize) {
                Field field;
                if (fields.empty())
                    count += field.read(trueData + count, settings);
                else
                    count += field.read(trueData + count, settings, fields.back());
                fields.push_back(field);
            }

            if (isCompressed(settings))
                delete [] trueData;  //Free what was allocated as decompData.

            return dataSize;
        }

        uint32_t read(char * buffer, const Settings& settings, bool doReadFields) {
            uint32_t count = readHeader(buffer, settings);

            if (doReadFields)
                readFields(buffer + count, settings);

            return count + dataSize;
        }
    };

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

    struct File : public Record {  // Inherited record is (assumed to be) TES4 or TES3.
        std::vector<Group> groups;
        std::vector<Record> records;

        File() {}

        File(const boost::filesystem::path& filepath, const Settings& settings, bool readFields, bool headerOnly) {

            ifstream input(filepath, std::ios::binary);
            input.exceptions(std::ios_base::badbit);

            input.seekg(0, input.end);
            int length = input.tellg();
            input.seekg(0, input.beg);

            //Allocate memory for file contents.
            char * buffer = new char[length];

            //Read whole file in.
            input.read(buffer, length);

            input.close();

            //First read the TES4/TES3 header.
            uint32_t count = Record::read(buffer, settings, true);

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

        std::vector<uint32_t> getFormIDs() {
            std::vector<uint32_t> formids;
            for (int i=0,max=groups.size(); i < max; ++i) {
                std::vector<uint32_t> fids = groups[i].getFormIDs();
                formids.insert(formids.end(), fids.begin(), fids.end());
            }
            for (int i=0,max=records.size(); i < max; ++i) {
                formids.push_back(records[i].id);
            }
            return formids;
        }

        bool isMaster(const Settings& settings) const {
            if (settings.record.mast_flag)
                return (flags & settings.record.mast_flag);
            else
                return false;
        }

        std::vector<std::string> getMasters() const {
            std::vector<std::string> masters;
            for(size_t i=0,max=fields.size(); i < max; ++i){
                if (strncmp(fields[i].type,"MAST", 4) == 0)
                    masters.push_back(fields[i].data);
            }
            return masters;
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
