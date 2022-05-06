/**
 * @file    Structs.h
 * @author  Samuel Martel
 * @date    2022-05-02
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
#ifndef NILAI_RTC_STRUCTS_H
#define NILAI_RTC_STRUCTS_H

#if defined(NILAI_USE_RTC)
#    include "../../defines/internal_config.h"
#    include NILAI_HAL_HEADER

#    include "enums.h"

#    include <cstdint>
#    include <string>

namespace Nilai::Rtc
{
/**
 * @brief   Contains the time information.
 *
 * Format options:
 *  - `%h`: Hours
 *  - `%m`: Minutes
 *  - `%s`: Seconds
 */
struct Time
{
    uint8_t hours   = 0;
    uint8_t minutes = 0;
    uint8_t seconds = 0;

    DayLightSaving dayLightSaving = DayLightSaving::None;

    Time() = default;
    Time(const RTC_TimeTypeDef& time);

    [[nodiscard]] size_t          ToSeconds() const;
    [[nodiscard]] RTC_TimeTypeDef ToHal() const;
    [[nodiscard]] std::string     ToStr() const;
};

struct Date
{
    uint16_t year  = 0;
    uint8_t  month = 0;    // Month is coded in BCD. 0x01 is January, 0x12 is December
    uint8_t  day   = 0;    // Day of the month, starts at 1.
    uint8_t  dotw  = 1;    // Day of the week, 1 is Monday, 7 is Sunday

    Date() = default;
    Date(uint16_t y, uint8_t m, uint8_t d, uint8_t wd = 1) : year(y), month(m), day(d), dotw(wd) {}
    Date(const RTC_DateTypeDef& date);

    [[nodiscard]] std::string DotWtoStr() const;
    [[nodiscard]] std::string MonthtoStr() const;

    [[nodiscard]] RTC_DateTypeDef ToHal() const;
    [[nodiscard]] std::string     ToStr() const;
};

struct Timestamp
{
    Date date;
    Time time;

    Timestamp() = default;
    Timestamp(const Date& d, const Time& t) : date(d), time(t) {}
#    if defined(NILAI_RTC_USE_STL)
    Timestamp(size_t epoch);

    [[nodiscard]] size_t ToEpoch() const;
#    endif
};
}    // namespace Nilai::Rtc
#endif

#endif    // NILAI_RTC_STRUCTS_H
