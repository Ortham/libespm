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

#ifndef __ESPM_TES3_TES3_RECORD__
#define __ESPM_TES3_TES3_RECORD__

#include "../generic/record.h"

namespace espm {
    namespace tes3 {
        namespace TES3 {
            struct MAST : public espm::Field {
                MAST(const espm::Field& field) : espm::Field(field) {}

                std::string getString() const {
                    return std::string(data, dataSize - 1);  //Data should be null terminated.
                }
            };

            struct HEDR : public espm::Field {
                HEDR(const espm::Field& field) : espm::Field(field) {}

                std::string getDescription() const {
                    return std::string(data + 40, dataSize - 44);
                }
            };

            struct Record : public espm::Record {
                Record(const espm::Record& record) : espm::Record(record) {}

                std::vector<std::string> getMasters() const {
                    std::vector<std::string> masters;
                    for (const auto &field : fields) {
                        if (strncmp(field.type, "MAST", 4) == 0)
                            masters.push_back(MAST(field).getString());
                    }
                    return masters;
                }

                std::string getDescription() const {
                    for (const auto &field : fields) {
                        if (strncmp(field.type, "HEDR", 4) == 0) {
                            return HEDR(field).getDescription();
                        }
                    }
                    return "";
                }
            };
        }
    }
}

#endif
