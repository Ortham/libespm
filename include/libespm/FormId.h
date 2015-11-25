/*
 * This file is part of libespm
 *
 * Copyright (C) 2015 Oliver Hamlet
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

#ifndef LIBESPM_FORM_ID
#define LIBESPM_FORM_ID

#include <cstdint>
#include <string>
#include <vector>
#include <cctype>

namespace libespm {
  class FormId {
  private:
    uint32_t objectIndex;
    std::string pluginName;
  public:
    inline FormId(const std::string& parentPluginName, const std::vector<std::string>& masters, uint32_t formId) {
      unsigned int modIndex = formId >> 24;
      objectIndex = formId & ~((uint32_t)modIndex << 24);

      if (modIndex >= masters.size())
        pluginName = parentPluginName;
      else
        pluginName = masters[modIndex];
    }

    inline uint32_t getObjectIndex() const {
      return objectIndex;
    }

    inline std::string getPluginName() const {
      return pluginName;
    }

    inline bool operator < (const FormId& rhs) const {
      if (objectIndex != rhs.getObjectIndex())
        return objectIndex < rhs.getObjectIndex();

      std::string rhsPluginName = rhs.getPluginName();
      return std::lexicographical_compare(begin(pluginName),
                                          end(pluginName),
                                          begin(rhsPluginName),
                                          end(rhsPluginName),
                                          [](char lhs, char rhs) {
        return tolower(lhs) < tolower(rhs);
      });
    }

    inline bool operator == (const FormId& rhs) const {
      std::string rhsPluginName = rhs.getPluginName();

      return objectIndex == rhs.getObjectIndex()
        && pluginName.size() == rhs.getPluginName().size()
        && std::equal(begin(pluginName), end(pluginName), begin(rhs.getPluginName()), [](char lhs, char rhs) {
        return tolower(lhs) == tolower(rhs);
      });
    }

    inline bool operator != (const FormId& rhs) const {
      return !(*this == rhs);
    }
  };
}

#endif
