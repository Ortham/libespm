/*
 * records.h
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

#ifndef __ESPM_RECORDS__
#define __ESPM_RECORDS__

#include "settings.h"
#include "streams.h"
#include "field.h"

#include <string>
#include <stdint.h>

#ifdef USING_ZLIB
#include <zlib.h>
#endif

namespace espm {

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
            return (flags & settings.record.comp_flag) == settings.record.comp_flag;
        }

        uint32_t readHeader(char * buffer, size_t length, const Settings& settings) {
            uint32_t headerSize =
                settings.record.type_len +
                settings.record.size_len +
                settings.record.unk1_len +
                settings.record.flags_len +
                settings.record.id_len +
                settings.record.rev_len +
                settings.record.ver_len +
                settings.record.unk2_len;

            if (length < headerSize)
                throw std::runtime_error("File shorter than expected.");

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

        uint32_t readFields(char * buffer, size_t length, const Settings& settings) {

            if (length < dataSize)
                throw std::runtime_error("File shorter than expected.");

            char * trueData = buffer;
            uint32_t trueDataSize = dataSize;
#ifdef USING_ZLIB
            if (isCompressed(settings)) {
                //Need to uncompress the data.
                uint32_t compSize = dataSize - 4;
                uint32_t trueDataSize = *reinterpret_cast<uint32_t*>(trueData);
                trueData += 4;

                char * decompData = new char[trueDataSize];

                int ret = uncompress((Bytef*)decompData, (uLongf*)&trueDataSize, (Bytef*)trueData, (uLong)compSize);

                trueData = decompData;
            }
#else
            if (!isCompressed(settings)) {
#endif

                uint32_t count = 0;
                while (count < trueDataSize) {
                    Field field;
                    if (fields.empty())
                        count += field.read(trueData + count, settings);
                    else
                        count += field.read(trueData + count, settings, fields.back());
                    fields.push_back(field);
                }
#ifndef USING_ZLIB
            }
#else
            if (isCompressed(settings))
                delete [] trueData;  //Free what was allocated as decompData.
#endif
            return dataSize;
        }

        uint32_t read(char * buffer, size_t length, const Settings& settings, bool doReadFields) {
            uint32_t count = readHeader(buffer, length, settings);

            if (doReadFields)
                readFields(buffer + count, length - count, settings);

            return count + dataSize;
        }

        uint32_t read(ifstream& in, const Settings& settings, bool doReadFields) {
            uint32_t headerSize =
                settings.record.type_len +
                settings.record.size_len +
                settings.record.unk1_len +
                settings.record.flags_len +
                settings.record.id_len +
                settings.record.rev_len +
                settings.record.ver_len +
                settings.record.unk2_len;

            //Allocate memory for record contents.
            char * buffer;
            try {
                buffer = new char[headerSize];
            } catch (std::bad_alloc& /*e*/) {
                return 0;
            }

            //Read header in.
            in.read(buffer, headerSize);

            //Now extract info from buffer.
            uint32_t count = readHeader(buffer, headerSize, settings);

            //Reallocate the buffer for the fields.
            delete [] buffer;
            try {
                buffer = new char[dataSize];
            } catch (std::bad_alloc& /*e*/) {
                return headerSize;
            }

            //Read fields in.
            in.read(buffer, dataSize);

            if (doReadFields)
                readFields(buffer, dataSize, settings);

            delete [] buffer;

            return count + dataSize;
        }
    };
}

#endif
