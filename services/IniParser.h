/**
 * @file    IniParser.h
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
#ifndef NILAI_INIPARSER_H
#define NILAI_INIPARSER_H

#if defined(NILAI_USE_INI_PARSER)
#    if !defined(NILAI_USE_FILESYSTEM)
#        error The filesystem module must be used!
#    else

#        include "../defines/IniParser/Concepts.h"
#        include "../defines/IniParser/Types.h"

#        include <map>
#        include <string>
namespace cep
{
class IniParser
{
public:
    using IniStruct = std::map<std::string, std::string>;
    using IniField  = std::pair<std::string, std::string>;

public:
    // Opens a file and parses it as a .ini file.
    explicit IniParser(std::string fp);

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
    [[nodiscard]] T Get(const std::string& section, const std::string& name, const T& def = {})
    {
        std::string f = GetField(section, name, "");
        return f.empty() ? def : StrToVal<T>(f);
    }

    void SetStr(const std::string& section, const std::string& name, const std::string& v);
    void SetBool(const std::string& section, const std::string& name, bool v);
    void SetInt(const std::string& section, const std::string& name, int32_t v);
    void SetUInt(const std::string& section, const std::string& name, uint32_t v);
    void SetDouble(const std::string& section, const std::string& name, double v);

    /**
     * @brief Returns true if the given @c section exists.
     * @note Section must contain at least one @c name=value pair.
     * @param section The section name
     * @returns True if the section exists
     * @returns False if the section does not exist.
     */
    [[nodiscard]] bool HasSection(const std::string& section) const;

    /**
     * @brief Returns true if a value exists for @c name in the @c section.
     * @param section The section name
     * @param name The value name
     * @returns True if the value exists
     * @returns False if the value does not exist
     */
    [[nodiscard]] bool HasValue(const std::string& section, const std::string& name) const;

    virtual IniStruct::iterator                     begin() { return m_values.begin(); }
    [[nodiscard]] virtual IniStruct::const_iterator cbegin() const { return m_values.cbegin(); }
    virtual IniStruct::reverse_iterator             rbegin() { return m_values.rbegin(); }
    [[nodiscard]] virtual IniStruct::const_reverse_iterator crbegin() const
    {
        return m_values.crbegin();
    }

    virtual IniStruct::iterator                     end() { return m_values.end(); }
    [[nodiscard]] virtual IniStruct::const_iterator cend() const { return m_values.cend(); }
    virtual IniStruct ::reverse_iterator            rend() { return m_values.rend(); }
    [[nodiscard]] virtual IniStruct ::const_reverse_iterator crend() const
    {
        return m_values.crend();
    }

    const std::string& operator[](const std::string& k) const { return m_values.at(k); }

private:
    /**
     * @brief Gets a string value from the INI file, returning @c def if not found.
     * @param section The section's name (<tt>[section]</tt>)
     * @param name The value's name (<tt>name=value</tt>)
     * @param def The default value, in case it isn't found
     * @return The value found, or the default value
     */
    [[nodiscard]] const std::string& GetField(const std::string& section,
                                              const std::string& name,
                                              const std::string& def = "") const;

private:
    std::string m_fp;

    bool                               m_isDirty = false;
    int                                m_error   = 0;
    std::map<std::string, std::string> m_values  = {};

    static std::string MakeKey(const std::string& section, const std::string& name);
    static int ValueHandler(void* usr, const char* section, const char* name, const char* value);
};
}    // namespace cep

#    endif
#endif

#endif    // NILAI_INIPARSER_H
