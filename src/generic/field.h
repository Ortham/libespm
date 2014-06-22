/*
 * fields.h
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

#ifndef __ESPM_FIELDS__
#define __ESPM_FIELDS__

#include "settings.h"

#include <string>
#include <cstdint>

namespace espm {

    struct Field {
        Field() : data(nullptr), dataSize(0) {}
        Field(const Field& field) : 
            dataSize(field.dataSize), 
            data(new char[dataSize])
        {
            memcpy(type, field.type, 4);
            memcpy(data, field.data, dataSize);
        }
        Field(Field&& field) :
            dataSize(field.dataSize),
            data(field.data)
        {
            field.dataSize = 0;
            field.data = nullptr;
            memcpy(type, field.type, 4);
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

        Field& operator = (Field&& rhs) {
            delete[] data;

            dataSize = rhs.dataSize;
            data = rhs.data;
            memcpy(type, rhs.type, 4);

            rhs.dataSize = 0;
            rhs.data = nullptr;
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
}

#endif
