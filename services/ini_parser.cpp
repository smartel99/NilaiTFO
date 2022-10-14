/**
 * @file    ini_parser.cpp
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
 * not, see <a href=https://www.gnu.org/licenses/>https://www.gnu.org/licenses/</a>.
 */
#if defined(NILAI_USE_INI_PARSER)
#    include "ini_parser.h"

#    include <utility>

#    include "../vendor/inih/ini.h"

#    include "filesystem.h"
#    include "logger.h"


namespace Nilai::Services
{
IniParser::IniParser(std::string_view fp) : m_fp(fp)
{
    using File      = Nilai::Filesystem::File;
    using FileModes = Nilai::Filesystem::FileModes;

    // Open the file first.
    File f(m_fp, FileModes::Read);
    if (!f.IsOpen())
    {
        LOG_ERROR("Unable to open '%s': (%i) %s",
                  m_fp.c_str(),
                  static_cast<int>(f.GetError()),
                  ResultToStr(f.GetError()));
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
    using File      = Nilai::Filesystem::File;
    using FileModes = Nilai::Filesystem::FileModes;

    File f(m_fp, FileModes::Write);
    if (!f.IsOpen())
    {
        LOG_ERROR("Unable to open '%s': (%i) %s",
                  m_fp.c_str(),
                  static_cast<int>(f.GetError()),
                  ResultToStr(f.GetError()));
        return;
    }

    // Find all the sections.
    struct Kvp
    {
        std::string K;
        std::string V;

        Kvp(std::string k, std::string v) : K(std::move(k)), V(std::move(v)) {}
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

void IniParser::SetStr(std::string_view section, std::string_view name, std::string_view v)
{
    m_isDirty                        = true;
    m_values[MakeKey(section, name)] = v;
}

void IniParser::SetBool(std::string_view section, std::string_view name, bool v)
{
    using namespace std::string_literals;
    SetStr(section, name, v ? "true"s : "false"s);
}

void IniParser::SetInt(std::string_view section, std::string_view name, int32_t v)
{
    SetStr(section, name, std::to_string(v));
}

void IniParser::SetUInt(std::string_view section, std::string_view name, uint32_t v)
{
    SetStr(section, name, std::to_string(v));
}

void IniParser::SetDouble(std::string_view section, std::string_view name, double v)
{
    SetStr(section, name, std::to_string(v));
}

std::string IniParser::GetField(std::string_view   section,
                                std::string_view   name,
                                const std::string& def) const
{
    std::string k = MakeKey(section, name);
    return m_values.count(k) != 0 ? m_values.at(k) : def;
}


bool IniParser::HasSection(std::string_view section) const
{
    using namespace std::string_view_literals;
    std::string key = MakeKey(section, ""sv);
    const auto& pos = m_values.lower_bound(key);
    if (pos == m_values.end())
    {
        return false;
    }
    // Does the key at the lower_bound pos start with "section"?
    return pos->first.compare(0, key.length(), key) == 0;
}

bool IniParser::HasValue(std::string_view section, std::string_view name) const
{
    std::string key = MakeKey(section, name);
    return m_values.count(key) != 0;
}

std::string IniParser::MakeKey(std::string_view section, std::string_view name)
{
    using namespace std::string_literals;
    std::string k;
    k.reserve(section.size() + name.size() + 1);
    k += section;
    k += '=';
    k += name;

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

std::string IniParser::GetSectionFromKey(const std::string& k)
{
    return std::string {k.substr(0, k.find_last_of('='))};
}

std::string IniParser::GetNameFromKey(const std::string& k)
{
    return std::string {k.substr(k.find_last_of('=') + 1)};
}
}    // namespace Nilai::Services
#endif
