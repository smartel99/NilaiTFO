/**
 * @file    ini_parser.h
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
#ifndef NILAI_INIPARSER_H
#define NILAI_INIPARSER_H

#if defined(NILAI_USE_INI_PARSER)
#    if !defined(NILAI_USE_FILESYSTEM)
static_assert(false, "The filesystem module must be used!");
#    else

#        include "../defines/ini_parser/concepts.h"
#        include "../defines/ini_parser/types.h"

#        include <map>
#        include <string>
#        include <string_view>

/**
 * @addtogroup Nilai
 * @{
 */

/**
 * @addtogroup Services
 * @{
 */

/**
 * @addtogroup nilai_ini Ini Parser
 * @{
 */

namespace Nilai::Services
{
class IniParser
{
public:
    using IniStruct = std::map<std::string, std::string>;
    using IniField  = std::pair<std::string, std::string>;

public:
    // Opens a file and parses it as a .ini file.
    explicit IniParser(std::string_view fp);

    /**
     * @brief Gets the result of ini_parse, i.e., 0 on success, line number of the first error, or
     * -1 on file open error.
     * @return The error code.
     */
    [[nodiscard]] int GetError() const { return m_error; }

    [[nodiscard]] bool IsDirty() const { return m_isDirty; }
    void               Save();

#        if defined(__cpp_concepts)
    template<IsIniType T>
#        else
    template<typename T>
#        endif
    [[nodiscard]] T Get(std::string_view section, std::string_view name, const T& def = {}) const
    {
        std::string f = GetField(section, name, "");
        return f.empty() ? def : StrToVal<T>(f);
    }

    void SetStr(std::string_view section, std::string_view name, std::string_view v);
    void SetBool(std::string_view section, std::string_view name, bool v);
    void SetInt(std::string_view section, std::string_view name, int32_t v);
    void SetUInt(std::string_view section, std::string_view name, uint32_t v);
    void SetDouble(std::string_view section, std::string_view name, double v);

    /**
     * @brief Returns true if the given @c section exists.
     * @note Section must contain at least one @c name=value pair.
     * @param section The section name
     * @returns True if the section exists
     * @returns False if the section does not exist.
     */
    [[nodiscard]] bool HasSection(std::string_view section) const;

    /**
     * @brief Returns true if a value exists for @c name in the @c section.
     * @param section The section name
     * @param name The value name
     * @returns True if the value exists
     * @returns False if the value does not exist
     */
    [[nodiscard]] bool HasValue(std::string_view section, std::string_view name) const;

    virtual IniStruct::iterator                     begin() { return m_values.begin(); }
    [[nodiscard]] virtual IniStruct::const_iterator cbegin() const { return m_values.cbegin(); }
    virtual IniStruct::reverse_iterator             rbegin() { return m_values.rbegin(); }
    [[nodiscard]] virtual IniStruct::const_reverse_iterator crbegin() const
    {
        return m_values.crbegin();
    }

    virtual IniStruct::iterator                             end() { return m_values.end(); }
    [[nodiscard]] virtual IniStruct::const_iterator         cend() const { return m_values.cend(); }
    virtual IniStruct::reverse_iterator                     rend() { return m_values.rend(); }
    [[nodiscard]] virtual IniStruct::const_reverse_iterator crend() const
    {
        return m_values.crend();
    }

    const std::string& operator[](const std::string& k) const { return m_values.at(k); }

    static std::string GetSectionFromKey(const std::string& k);
    static std::string GetNameFromKey(const std::string& k);

private:
    /**
     * @brief Gets a string value from the INI file, returning @c def if not found.
     * @param section The section's name (<tt>[section]</tt>)
     * @param name The value's name (<tt>name=value</tt>)
     * @param def The default value, in case it isn't found
     * @return The value found, or the default value
     */
    [[nodiscard]] std::string GetField(std::string_view   section,
                                       std::string_view   name,
                                       const std::string& def = "") const;

private:
    std::string m_fp;

    bool      m_isDirty = false;
    int       m_error   = 0;
    IniStruct m_values  = {};

    static std::string MakeKey(std::string_view section, std::string_view name);
    static int ValueHandler(void* usr, const char* section, const char* name, const char* value);
};
}    // namespace Nilai::Services

//!@}
//!@}
//!@}
#    endif
#endif
#endif    // NILAI_INIPARSER_H
