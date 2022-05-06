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
#include "ini_parser.h"

#if defined(NILAI_USE_INI_PARSER)
#    include "../vendor/inih/ini.h"

#    include "filesystem.h"
#    include "logger.hpp"

static std::string GetSectionFromKey(const std::string& k);
static std::string GetNameFromKey(const std::string& k);

namespace Nilai
{
IniParser::IniParser(std::string fp) : m_fp(std::move(fp))
{
    using namespace Nilai::Filesystem;
    // Open the file first.
    File f(m_fp, FileModes::Read);
    if (!f.IsOpen())
    {
        LOG_ERROR("Unable to open '%s': (%i) %s",
                  m_fp.c_str(),
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

    f.Close();

    m_error = ini_parse_string(file.c_str(), ValueHandler, this);
}

void IniParser::Save()
{
    using namespace Nilai::Filesystem;

    File f(m_fp, FileModes::Write);
    if (!f.IsOpen())
    {
        LOG_ERROR("Unable to open '%s': (%i) %s",
                  m_fp.c_str(),
                  (int)f.GetError(),
                  ResultToStr(f.GetError()).c_str());
        return;
    }

    // Find all the sections.
    struct Kvp
    {
        std::string K;
        std::string V;

        Kvp(const std::string& k, const std::string& v) : K(k), V(v) {}
    };

    std::map<std::string, std::vector<Kvp>> data;
    for (const auto& [secNName, v] : m_values)
    {
        data[GetSectionFromKey(secNName)].emplace_back(GetNameFromKey(secNName), v);
    }


    // Dump the sections and key-value pairs.
    for (const auto& [section, kvp] : data)
    {
        f.WriteFmtString("[%s]\n", section.c_str());
        for (const auto& [k, v] : kvp)
        {
            f.WriteFmtString("%s=%s\n", k.c_str(), v.c_str());
        }
        f.WriteString("\n");
    }

    f.Close();
    m_isDirty = false;
}

void IniParser::SetStr(const std::string& section, const std::string& name, const std::string& v)
{
    m_isDirty                        = true;
    m_values[MakeKey(section, name)] = v;
}

void IniParser::SetBool(const std::string& section, const std::string& name, bool v)
{
    SetStr(section, name, std::string {v ? "true" : "false"});
}

void IniParser::SetInt(const std::string& section, const std::string& name, int32_t v)
{
    SetStr(section, name, std::to_string(v));
}

void IniParser::SetUInt(const std::string& section, const std::string& name, uint32_t v)
{
    SetStr(section, name, std::to_string(v));
}

void IniParser::SetDouble(const std::string& section, const std::string& name, double v)
{
    SetStr(section, name, std::to_string(v));
}

const std::string& IniParser::GetField(const std::string& section,
                                       const std::string& name,
                                       const std::string& def) const
{
    std::string k = MakeKey(section, name);
    return m_values.count(k) != 0 ? m_values.at(k) : def;
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

}    // namespace Nilai

std::string GetSectionFromKey(const std::string& k)
{
    return k.substr(0, k.find_last_of('='));
}

std::string GetNameFromKey(const std::string& k)
{
    return k.substr(k.find_last_of('=') + 1);
}


#endif
