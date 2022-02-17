/**
 * @file    IniParser.cpp
 * @author  Samuel Martel
 * @date    2022-02-16
 * @brief
 *
 * @copyright
 * This program is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If
 * not, see <a href=https://www.gnu.org/licenses/>https://www.gnu.org/licenses/<a/>.
 */
#include "IniParser.h"

#if defined(NILAI_USE_INI_PARSER)
#    include "../vendor/inih/ini.h"

#    include "filesystem.h"
#    include "logger.hpp"

namespace cep
{

IniParser::IniParser(const std::string& fp)
{
    using namespace cep::Filesystem;
    // Open the file first.
    File f(fp, FileModes::Read);
    if (!f.IsOpen())
    {
        LOG_ERROR("Unable to open '%s': (%i) %s",
                  fp.c_str(),
                  (int)f.GetError(),
                  ResultToStr(f.GetError()).c_str());
        // Unable to open the file.
        m_error = -1;
        return;
    }

    // Read the entire file into a string.
    std::string file;
    file.reserve(f.GetSize());

    while (!f.AtEoF())
    {
        std::string l;
        f.GetString(l, INI_MAX_LINE);
        file += l;
    }

    m_error = ini_parse_string(file.c_str(), ValueHandler, this);
}

IniParser::IniParser(const char* buff, size_t len)
{
    std::string c(buff, len);
    m_error = ini_parse(c.c_str(), ValueHandler, this);
}

const std::string& IniParser::Get(const std::string& section,
                                  const std::string& name,
                                  const std::string& def) const
{
    std::string k = MakeKey(section, name);
    return m_values.count(k) != 0 ? m_values.at(k) : def;
}

std::string IniParser::GetString(const std::string& section,
                                 const std::string& name,
                                 const std::string& def) const
{
    const std::string& s = Get(section, name);
    return s.empty() ? def : s;
}

uint32_t IniParser::GetInteger(const std::string& section,
                               const std::string& name,
                               uint32_t           def) const
{
    const std::string& valstr = Get(section, name);
    const char*        value  = valstr.c_str();
    char*              end;
    // This parses "1234" (decimal) and also "0x4D2" (hex)
    uint32_t n = strtol(value, &end, 0);
    return end > value ? n : def;
}

double IniParser::GetDecimal(const std::string& section, const std::string& name, double def) const
{
    const std::string& valstr = Get(section, name);
    const char*        value  = valstr.c_str();
    char*              end;
    double             n = strtod(value, &end);
    return end > value ? n : def;
}

bool IniParser::GetBoolean(const std::string& section, const std::string& name, bool def) const
{
    std::string valstr = Get(section, name);
    // Convert to lower case to make string comparisons case-insensitive
    std::transform(valstr.begin(), valstr.end(), valstr.begin(), ::tolower);
    if (valstr == "true" || valstr == "yes" || valstr == "on" || valstr == "1")
    {
        return true;
    }
    else if (valstr == "false" || valstr == "no" || valstr == "off" || valstr == "0")
    {
        return false;
    }
    else
    {
        return def;
    }
}

bool IniParser::HasSection(const std::string& section) const
{
    std::string key = MakeKey(section, "");
    const auto& pos = m_values.lower_bound(key);
    if (pos == m_values.end())
    {
        return false;
    }
    // Does the key at the lower_bound pos start with "section"?
    return pos->first.compare(0, key.length(), key) == 0;
}

bool IniParser::HasValue(const std::string& section, const std::string& name) const
{
    std::string key = MakeKey(section, name);
    return m_values.count(key) != 0;
}

std::string IniParser::MakeKey(const std::string& section, const std::string& name)
{
    std::string k = section + "=" + name;
    // convert to lower case to make section/name lookups case-insensitive.
    std::transform(k.begin(), k.end(), k.begin(), ::tolower);

    return k;
}

int IniParser::ValueHandler(void* usr, const char* section, const char* name, const char* value)
{
    // Can happen if INI_CALL_HANDLER_ON_NEW_SECTION is enabled.
    if (name == nullptr)
    {
        return 1;    // Still valid.
    }

    IniParser*  reader = static_cast<IniParser*>(usr);
    std::string key    = MakeKey(section, name);
    if (!reader->m_values[key].empty())
    {
        // Value already exists, add a new line.
        reader->m_values[key] += "\n";
    }

    // Add the value to the entry.
    reader->m_values[key] += value != nullptr ? value : "";
    return 1;
}

}    // namespace cep
#endif
