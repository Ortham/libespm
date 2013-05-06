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
#include <fstream>

#include <yaml-cpp/yaml.h>
#include <zlib.h>

/*
    Libespm playground for BOSSv3. Useful things will be implemented properly into my libespm fork. Try to improve performance by reading record and group headers into memory as a block.

    Also need to implement getRecords, getRecordByFieldData, getRecordByID, getGroupByType and getGroups.

    
*/

namespace espm {

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

        uint32_t read(std::ifstream &input, const Settings& settings, const Field& lastField) {

            input.read(type, settings.field.type_len);
            input.read(reinterpret_cast<char*>(&dataSize), settings.field.size_len);

            if (strncmp(lastField.type,"XXXX", 4) == 0)
                memcpy(&dataSize, lastField.data, 4);

            data = new char[dataSize];
            input.read(data, dataSize);

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

        uint32_t read(std::ifstream &input, const Settings& settings, bool readFields) {
            input.read(type, settings.record.type_len);
            input.read(reinterpret_cast<char*>(&dataSize), settings.record.size_len);
            input.read(reinterpret_cast<char*>(&unknown1), settings.record.unk1_len);
            input.read(reinterpret_cast<char*>(&flags), settings.record.flags_len);
            input.read(reinterpret_cast<char*>(&id), settings.record.id_len);
            input.read(reinterpret_cast<char*>(&revision), settings.record.rev_len);
            input.read(reinterpret_cast<char*>(&version), settings.record.ver_len);
            input.read(reinterpret_cast<char*>(&unknown2), settings.record.unk2_len);

            uint32_t count = 0;
            if (readFields) {
                if (isCompressed(settings)) {

                    uint32_t compSize = dataSize - 4;
                    uint32_t decompSize;
                    
                    input.read(reinterpret_cast<char*>(&decompSize), 4);

                    char * decompData = new char[decompSize];
                    char * compData = new char[compSize];
                    
                    input.read(compData, compSize);

                    int ret = uncompress((Bytef*)decompData, (uLongf*)&decompSize, (Bytef*)compData, (uLong)compSize);

                    while (count < decompSize) {
                        Field field, last;
                        if (!fields.empty())
                            last = fields.back();
                        else
                            memcpy(last.type, "DMMY", 4);  //Initialise the type so nothing weird happens when it gets compared.
                        count += field.read(decompData + count, settings, last);
                        fields.push_back(field);
                    }

                    count = dataSize;
                } else {
                    while (count < dataSize) {
                        Field field, last;
                        if (!fields.empty())
                            last = fields.back();
                        else
                            memcpy(last.type, "DMMY", 4);  //Initialise the type so nothing weird happens when it gets compared.
                        count += field.read(input, settings, last);
                        fields.push_back(field);
                    }
                }
            } else {
                //Check distance to end of file.
                uint32_t pos = input.tellg();
                input.seekg(0, std::ios_base::end);
                uint32_t pos2 = input.tellg();
                if (dataSize <= pos2 - pos)
                    input.seekg(pos + dataSize, std::ios_base::beg);
                count = dataSize;
            }
                

            return (uint32_t)settings.record.type_len + settings.record.size_len + settings.record.unk1_len + settings.record.flags_len + settings.record.id_len + settings.record.rev_len + settings.record.ver_len + settings.record.unk2_len + count;
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

        std::vector<uint32_t> formids;
        std::vector<Record> records;
        std::vector<Group> subgroups;

        uint32_t read(std::ifstream &input, const Settings& settings, bool readFields) {
            try {
                input.read(type, settings.group.type_len);
            } catch (std::ifstream::failure &e) {
                return 0;
            }
            input.read(reinterpret_cast<char*>(&groupSize), settings.group.size_len);
            input.read(label, settings.group.label_len);
            input.read(reinterpret_cast<char*>(&groupType), settings.group.groupType_len);
            input.read(reinterpret_cast<char*>(&stamp), settings.group.stamp_len);
            input.read(reinterpret_cast<char*>(&unknown1), settings.group.unk1_len);
            input.read(reinterpret_cast<char*>(&version), settings.group.ver_len);
            input.read(reinterpret_cast<char*>(&unknown2), settings.group.unk1_len);

            uint32_t count = settings.group.type_len + settings.group.size_len + settings.group.label_len + settings.group.groupType_len + settings.group.stamp_len + settings.group.unk1_len + settings.group.unk2_len + settings.group.ver_len;

            while (count < groupSize && input.good()) {
                char temp[4];

                input.read(temp, settings.record.type_len);

                for (int i=0; i < settings.record.type_len; ++i)
                    input.unget();

                if (strncmp(temp,settings.group.type.data(), 4) == 0) {
                    Group subgroup;
                    count += subgroup.read(input, settings, readFields);
                    subgroups.push_back(subgroup);
                } else {
                    Record record;
                    count += record.read(input, settings, readFields);
                    records.push_back(record);
                }
            }

            return count;

        }

        std::vector<uint32_t> getFormIDs() {
            if (formids.empty()) {
                for (int i=0,max=subgroups.size(); i < max; ++i) {
                    std::vector<uint32_t> fids = subgroups[i].getFormIDs();
                    formids.insert(formids.end(), fids.begin(), fids.end());
                }
                for (int i=0,max=records.size(); i < max; ++i) {
                    formids.push_back(records[i].id);
                }
            }
            return formids;
        }

        Record getRecordByFieldData(char * name, char * data, uint32_t dataLength) const {
            Record record;
            return record;
        }

        std::vector<Record> getRecords() const {
            std::vector<Record> records;
            return records;
        }

        Record getRecordByID(uint32_t id) const {
            Record record;
            return record;
        }
    };

    struct File : public Record {  // Inherited record is (assumed to be) TES4 or TES3.
        std::vector<uint32_t> formids;
        std::vector<Group> groups;

        File() {}
        File(std::ifstream &input, const Settings& settings, bool readFields, bool headerOnly) {
                
            Record::read(input, settings, true);  //Always read header fields.

            if (!headerOnly) {
                while (input.good()) {
                    Group group;
                    group.read(input, settings, readFields);
                    groups.push_back(group);
                }
            }
        }

        File(const std::string& filepath, const Settings& settings, bool readFields, bool headerOnly) {

            std::ifstream input(filepath.c_str(), std::ios::binary);
            input.exceptions(std::ifstream::failbit | std::ifstream::badbit);
                
            Record::read(input, settings, true);  //Always read header fields.

            if (!headerOnly) {
                while (input.good()) {
                    Group group;
                    group.read(input, settings, readFields);
                    groups.push_back(group);
                }
            }

            input.close();
        }

        std::vector<uint32_t> getFormIDs() {
            if (formids.empty()) {
                for (int i=0,max=groups.size(); i < max; ++i) {
                    std::vector<uint32_t> fids = groups[i].getFormIDs();
                    formids.insert(formids.end(), fids.begin(), fids.end());
                }
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

        Record getRecordByFieldData(char * name, char * data, uint32_t dataLength) const {
            Record record;
            return record;
        }

        std::vector<Record> getRecords() const {
            std::vector<Record> records;
            return records;
        }

        Record getRecordByID(uint32_t id) const {
            Record record;
            return record;
        }

        Group getGroupByType(char * type) const {
            Group group;
            return group;
        }

        std::vector<Group> getGroups() const {
            std::vector<Group> groups;
            return groups;
        }
    };

}

#endif
