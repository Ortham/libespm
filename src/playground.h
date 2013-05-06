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

#include <iostream>

#include <yaml-cpp/yaml.h>
#include <zlib.h>

/*
    Libespm playground for BOSSv3. Useful things will be implemented properly into my libespm fork.
    Try to get full plugin data read implemented with reasonable performance (speed/memory) impact - mem usage of 2x plugin filesizes is upper limit.

    Inter-game differences:

        - Oblivion records have no version or unknown field.
        - Oblivion groups's stamp is 4 bytes long and groups have no version, unknown1 or unknown2 fields.
        - Morrowind has no groups at all.
        - Morrowind records have a 4 byte unknown field inserted before the flags field, and no id, revision, version or unknown fields following it.

    Need to implement the following libespm functionality:

        - getRecords()
        - getRecordByFieldD - checks type, size and data.
        - getRecordByID - checks id.
        - getGroupByName - checks type
        - getGroups

    
*/

namespace espm {
    

    struct Settings {
        Settings() {}
        Settings(const std::string& filepath, const std::string& game) {
            
            YAML::Node contents = YAML::LoadFile(filepath);  //Throws exception on failure.

            YAML::Node ys = contents[game];

            length = ys["Length"].as<unsigned int>();
            field_size_length = ys["FieldSizeLength"].as<unsigned int>();
            flag_length = ys["FlagLength"].as<unsigned int>();
            comp_flag = ys["CompFlag"].as<uint32_t>();
            mast_flag = ys["MastFlag"].as<uint32_t>();

            if (ys["IDLength"])
                id_length = ys["IDLength"].as<unsigned int>();
            if (ys["RevLength"])
                rev_length = ys["RevLength"].as<unsigned int>();
            if (ys["VerLength"])
                ver_length = ys["VerLength"].as<unsigned int>();
            if (ys["StuffzLength"])
                unknown_length = ys["StuffzLength"].as<unsigned int>();
            if (ys["HeadSizeLength"])
                head_size_length = ys["HeadSizeLength"].as<unsigned int>();
            if (ys["RecSizeLength"])
                rec_size_length = ys["RecSizeLength"].as<unsigned int>();
            if (ys["DecompSizeLength"])
                decomp_size_length = ys["DecompSizeLength"].as<unsigned int>();
            if (ys["GroupTypeLength"])
                group_type_length = ys["GroupTypeLength"].as<unsigned int>();
            if (ys["GroupStampLength"])
                group_stamp_length = ys["GroupStampLength"].as<unsigned int>();
            if (ys["GroupSizeLength"])
                group_size_length = ys["GroupSizeLength"].as<unsigned int>();
            
            bsa_head = ys["BSAHead"].as<std::string>();
            plug_head = ys["PlugHead"].as<std::string>();
            mast_ext = ys["MastExt"].as<std::string>();
            plug_ext = ys["PlugExt"].as<std::string>();
            save_ext = ys["SaveExt"].as<std::string>();
            group = ys["Group"].as<std::string>();
            if (ys["SaveHead"])
                save_head = ys["SaveHead"].as<std::string>();
            
            onam = ys["ONAM"].as< std::vector<std::string> >();
        }
        
        unsigned int length;
        unsigned int field_size_length;
        unsigned int flag_length;
        uint32_t comp_flag;
        uint32_t mast_flag;
        unsigned int id_length;
        unsigned int rev_length;
        unsigned int ver_length;
        unsigned int unknown_length;
        unsigned int head_size_length;
        unsigned int rec_size_length;
        unsigned int decomp_size_length;
        unsigned int group_type_length;
        unsigned int group_stamp_length;
        unsigned int group_size_length;

        std::string bsa_head;
        std::string plug_head;
        std::string save_head;
        std::string mast_ext;
        std::string plug_ext;
        std::string save_ext;
        std::string group;

        std::vector<std::string> onam;
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

        uint32_t Read(std::ifstream &input, const Settings& settings, const Field& lastField) {

            input.read(type, settings.length);
            input.read(reinterpret_cast<char*>(&dataSize), settings.field_size_length);

            if (strncmp(lastField.type,"XXXX", 4) == 0)
                memcpy(&dataSize, lastField.data, 4);

    //        std::cout << "Subrecord Type: " << std::string(type, 4) << std::endl;

            data = new char[dataSize];
            input.read(data, dataSize);

            return (uint32_t)settings.length + settings.field_size_length + dataSize;
        }

        uint32_t Read(char * buffer, const Settings& settings, const Field& lastField) {

            memcpy(type, buffer, settings.length);
            memcpy(&dataSize, buffer + settings.length, settings.field_size_length);
            
            if (strncmp(lastField.type,"XXXX", 4) == 0)
                memcpy(&dataSize, lastField.data, 4);

    //        std::cout << "Subrecord Type: " << std::string(type, 4) << std::endl;

            data = new char[dataSize];
            memcpy(data, buffer + settings.length + settings.field_size_length, dataSize);

            
            return (uint32_t)settings.length + settings.field_size_length + dataSize;
        }
    };

    struct Record {
        char type[4];
        
        uint32_t dataSize;
        uint32_t flags;
        uint32_t id;
        uint32_t revision;
        
        uint16_t version;
        uint16_t unknown;

        std::vector<Field> fields;

        bool isCompressed(const Settings& settings) const {
            return flags & settings.comp_flag;
        }

        uint32_t Read(std::ifstream &input, const Settings& settings) {
            input.read(type, settings.length);
            input.read(reinterpret_cast<char*>(&dataSize), settings.head_size_length);
            input.read(reinterpret_cast<char*>(&flags), settings.flag_length);
            input.read(reinterpret_cast<char*>(&id), settings.id_length);
            input.read(reinterpret_cast<char*>(&revision), settings.rev_length);
            input.read(reinterpret_cast<char*>(&version), settings.ver_length);
            input.read(reinterpret_cast<char*>(&unknown), settings.unknown_length);

   //         std::cout << "Record Type: " << std::string(type, 4) << std::endl;
   //         std::cout << "FormID: " << std::hex << id << std::dec << std::endl;
   //         std::cout << "Is Compressed: " << (flags & settings.comp_flag) << std::endl;

            uint32_t count = 0;
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
                    count += field.Read(decompData + count, settings, last);
                    fields.push_back(field);
                }

 /*               uint32_t pos = input.tellg();
                input.seekg(0, std::ios_base::end);
                uint32_t pos2 = input.tellg();
                if (dataSize <= pos2 - pos)
                    input.seekg(pos + dataSize, std::ios_base::beg);
*/
                count = dataSize;
            } else {
                while (count < dataSize) {
                    Field field, last;
                    if (!fields.empty())
                        last = fields.back();
                    else
                        memcpy(last.type, "DMMY", 4);  //Initialise the type so nothing weird happens when it gets compared.
                    count += field.Read(input, settings, last);
                    fields.push_back(field);
                }
            }

            return (uint32_t)settings.length + settings.head_size_length + settings.flag_length + settings.id_length + settings.rev_length + settings.ver_length + settings.unknown_length + count;
        }

        uint32_t ReadFID(std::ifstream &input, const Settings& settings) {
            input.read(type, settings.length);
            input.read(reinterpret_cast<char*>(&dataSize), settings.head_size_length);
            input.read(reinterpret_cast<char*>(&flags), settings.flag_length);
            input.read(reinterpret_cast<char*>(&id), settings.id_length);

            //Check distance to end of file.
            uint32_t pos = input.tellg();
            input.seekg(0, std::ios_base::end);
            uint32_t pos2 = input.tellg();
            if (8 + dataSize <= pos2 - pos)
                input.seekg(pos + 8 + dataSize, std::ios_base::beg);
                
            return uint32_t(24) + dataSize;
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

        uint32_t Read(std::ifstream &input, const Settings& settings) {
            try {
                input.read(type, settings.length);
            } catch (std::ifstream::failure &e) {
                return 0;
            }
            input.read(reinterpret_cast<char*>(&groupSize), settings.group_size_length);
            input.read(label, settings.length);
            input.read(reinterpret_cast<char*>(&groupType), settings.group_type_length);
            input.read(reinterpret_cast<char*>(&stamp), settings.group_stamp_length);
            input.read(reinterpret_cast<char*>(&unknown1), settings.unknown_length);
            input.read(reinterpret_cast<char*>(&version), settings.ver_length);
            input.read(reinterpret_cast<char*>(&unknown2), settings.unknown_length);

            uint32_t count = settings.length * 2 + settings.group_size_length + settings.group_type_length + settings.group_stamp_length + settings.unknown_length * 2 + settings.ver_length;

            while (count < groupSize && input.good()) {
                char temp[4];

                input.read(temp, settings.length);

                for (int i=0; i < settings.length; ++i)
                    input.unget();

                if (strncmp(temp,settings.group.data(), 4) == 0) {
                    Group subgroup;
                    count += subgroup.Read(input, settings);
                    subgroups.push_back(subgroup);
                } else {
                    Record record;
                    count += record.Read(input, settings);
                    records.push_back(record);
                }
            }

            return count;

        }

        uint32_t ReadFIDs(std::ifstream &input, const Settings& settings) {
            try {
                input.read(type, settings.length);
            } catch (std::ifstream::failure &e) {
                return 0;
            }
            input.read(reinterpret_cast<char*>(&groupSize), settings.group_size_length);

            uint32_t count = settings.length + settings.group_type_length + settings.group_stamp_length + settings.unknown_length * 2 + settings.ver_length;

            input.seekg(count, std::ios_base::cur);

            count += settings.length + settings.group_size_length;

            while (count < groupSize && input.good()) {
                char temp[4];

                input.read(temp, settings.length);

                for (int i=0; i < settings.length; ++i)
                    input.unget();

                if (strncmp(temp,settings.group.data(), 4) == 0) {
                    Group subgroup;
                    count += subgroup.ReadFIDs(input, settings);
                    formids.insert(formids.end(), subgroup.formids.begin(), subgroup.formids.end());
                } else {
                    Record record;
                    count += record.ReadFID(input, settings);
                    formids.push_back(record.id);
                }
            }

            return count;
        }

        std::vector<uint32_t> GetFormIDs() {
            if (formids.empty()) {
                for (int i=0,max=subgroups.size(); i < max; ++i) {
                    std::vector<uint32_t> fids = subgroups[i].GetFormIDs();
                    formids.insert(formids.end(), fids.begin(), fids.end());
                }
                for (int i=0,max=records.size(); i < max; ++i) {
                    formids.push_back(records[i].id);
                }
            }
            return formids;
        }
    };

    struct File : public Record {  // Inherited record is TES4.
        std::vector<uint32_t> formids;
        std::vector<Group> groups;

        void Read(std::ifstream &input, const Settings& settings) {
                
            Record::Read(input, settings);

            while (input.good()) {
                Group group;
                group.Read(input, settings);
                groups.push_back(group);
            }
            
        }

        void ReadFIDs(std::ifstream &input, const Settings& settings) {
                
            Record::Read(input, settings);

            while (input.good()) {
                Group group;
                group.ReadFIDs(input, settings);
                
                formids.insert(formids.end(), group.formids.begin(), group.formids.end());
            }
            
        }

        void ReadHeader(std::ifstream &input, const Settings& settings) {
            Record::Read(input, settings);
        }

        std::vector<uint32_t> GetFormIDs() {
            if (formids.empty()) {
                for (int i=0,max=groups.size(); i < max; ++i) {
                    std::vector<uint32_t> fids = groups[i].GetFormIDs();
                    formids.insert(formids.end(), fids.begin(), fids.end());
                }
            }
            return formids;
        }

        bool IsMaster(const Settings& settings) const {
            return (flags & settings.mast_flag);
        }

        std::vector<std::string> GetMasters() const {
            std::vector<std::string> masters;
            for(size_t i=0,max=fields.size(); i < max; ++i){
                if (strncmp(fields[i].type,"MAST", 4) == 0)
                    masters.push_back(fields[i].data);
            }
            return masters;
        }
    };

}

#endif
