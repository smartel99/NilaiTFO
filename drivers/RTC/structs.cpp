/**
 * @file    structs.cpp
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
 * not, see <a href=https://www.gnu.org/licenses/>https://www.gnu.org/licenses/</a>.
 */
#if defined(NILAI_USE_RTC) && !defined(NILAI_TEST)
#    include "structs.h"

#    include <ctime>

namespace Nilai::Rtc
{
Time::Time(const RTC_TimeTypeDef& time)
: hours(time.Hours), minutes(time.Minutes), seconds(time.Seconds)
{
    switch (time.DayLightSaving)
    {
        case RTC_DAYLIGHTSAVING_SUB1H: dayLightSaving = Nilai::Rtc::DayLightSaving::SubOne; break;
        case RTC_DAYLIGHTSAVING_ADD1H: dayLightSaving = Nilai::Rtc::DayLightSaving::AddOne; break;
        case RTC_DAYLIGHTSAVING_NONE: [[fallthrough]];
        default: dayLightSaving = Nilai::Rtc::DayLightSaving::None; break;
    }
}

RTC_TimeTypeDef Time::ToHal() const
{
    RTC_TimeTypeDef time = {};
    time.Hours           = hours;
    time.Minutes         = minutes;
    time.Seconds         = seconds;
    time.TimeFormat = RTC_HOURFORMAT12_AM;    // We're in 24h mode, this doesn't impact anything.
    time.StoreOperation = RTC_STOREOPERATION_SET;
    switch (dayLightSaving)
    {
        case Nilai::Rtc::DayLightSaving::SubOne:
            time.DayLightSaving = RTC_DAYLIGHTSAVING_SUB1H;
            break;
        case Nilai::Rtc::DayLightSaving::AddOne:
            time.DayLightSaving = RTC_DAYLIGHTSAVING_ADD1H;
            break;
        case Nilai::Rtc::DayLightSaving::None: [[fallthrough]];
        default: time.DayLightSaving = RTC_DAYLIGHTSAVING_NONE; break;
    }

    return time;
}

std::string Time::ToStr() const
{
    char t[12] = {0};

    sprintf(t, "%02i:%02i:%02i", hours, minutes, seconds);

    return std::string {t};
}

size_t Time::ToSeconds() const
{
    return (hours * 3600) + (minutes * 60) + seconds;
}

Date::Date(const RTC_DateTypeDef& date)
: year((uint16_t)(date.Year + 2000)), month(date.Month), day(date.Date), dotw(date.WeekDay)
{
}

std::string Date::DotWtoStr() const
{
    switch (dotw)
    {
        case RTC_WEEKDAY_MONDAY: return "MON";
        case RTC_WEEKDAY_TUESDAY: return "TUE";
        case RTC_WEEKDAY_WEDNESDAY: return "WED";
        case RTC_WEEKDAY_THURSDAY: return "THU";
        case RTC_WEEKDAY_FRIDAY: return "FRI";
        case RTC_WEEKDAY_SATURDAY: return "SAT";
        case RTC_WEEKDAY_SUNDAY: return "SUN";
        default: return "ERR";
    }
}

std::string Date::MonthtoStr() const
{
    switch (month)
    {
        case RTC_MONTH_JANUARY: return "JAN";
        case RTC_MONTH_FEBRUARY: return "FEB";
        case RTC_MONTH_MARCH: return "MAR";
        case RTC_MONTH_APRIL: return "APR";
        case RTC_MONTH_MAY: return "MAY";
        case RTC_MONTH_JUNE: return "JUN";
        case RTC_MONTH_JULY: return "JUL";
        case RTC_MONTH_AUGUST: return "AUG";
        case RTC_MONTH_SEPTEMBER: return "SEP";
        case RTC_MONTH_OCTOBER: return "OCT";
        case RTC_MONTH_NOVEMBER: return "NOV";
        case RTC_MONTH_DECEMBER: return "DEC";
        default: return "ERR";
    }
}

RTC_DateTypeDef Date::ToHal() const
{
    RTC_DateTypeDef date;

    date.Date    = day;
    date.Month   = month;
    date.Year    = (uint8_t)(year - 2000);
    date.WeekDay = dotw;

    return date;
}

std::string Date::ToStr() const
{
    char d[14] = {0};

    sprintf(d, "%02i-%02i-%02i", year - 2000, month, day);

    return std::string {d};
}

#    if defined(NILAI_RTC_USE_STL)
Nilai::Rtc::Timestamp::Timestamp(size_t epoch)
{
    time_t     e = epoch;
    struct tm* tim;

    tim          = gmtime(&e);
    date.year    = (uint16_t)(tim->tm_year + 1900);
    date.month   = (uint8_t)(tim->tm_mon + 1);
    date.day     = (uint8_t)(tim->tm_mday);
    time.hours   = (uint8_t)(tim->tm_hour);
    time.minutes = (uint8_t)(tim->tm_min);
    time.seconds = (uint8_t)(tim->tm_sec);
}

size_t Nilai::Rtc::Timestamp::ToEpoch() const
{
    time_t    epoch = {0};
    struct tm tim   = {};

    tim.tm_year = (date.year - 1900);
    tim.tm_mon  = date.month - 1;
    tim.tm_mday = date.day;
    tim.tm_hour = time.hours;
    tim.tm_min  = time.minutes;
    tim.tm_sec  = time.seconds;

    epoch = mktime(&tim);
    return (size_t)epoch;
}
#    endif
}    // namespace Nilai::Rtc

#endif
