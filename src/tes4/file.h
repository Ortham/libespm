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

#ifndef __ESPM_TES4_FILE__
#define __ESPM_TES4_FILE__

#include "../generic/file.h"
#include "tes4.h"

namespace espm { namespace tes4 {

    struct File : public espm::File {

        File(const boost::filesystem::path filepath, const Settings& settings, bool readFields, bool headerOnly) : espm::File(filepath, settings, readFields, headerOnly) {}

        // The TES4 record is the file header, and so its data is also file metadata.

        bool isMaster(const Settings& settings) const {
            return (records.at(0).flags & settings.record.mast_flag);
        }

        std::vector<std::string> getMasters() const {
            return TES4::Record(records.at(0)).getMasters();
        }

        std::string getDescription() const {
            return TES4::Record(records.at(0)).getDescription();
        }
    };

} }

#endif
