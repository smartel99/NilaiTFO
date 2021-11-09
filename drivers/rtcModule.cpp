/**
 * @addtogroup rtcModule.cpp
 * @{
 *******************************************************************************
 * @file	rtcModule.cpp
 * @author	Samuel Martel
 * @brief
 * Created on: Apr 9, 2021
 *******************************************************************************
 */

#include "rtcModule.h"
#if defined(NILAI_USE_RTC) && defined(HAL_RTC_MODULE_ENABLED)
#    include "services/logger.hpp"

#    include <ctime>

namespace CEP_RTC
{
Time::Time(const RTC_TimeTypeDef& time)
: hours(time.Hours), minutes(time.Minutes), seconds(time.Seconds)
{
    switch (time.DayLightSaving)
    {
        case RTC_DAYLIGHTSAVING_SUB1H: dayLightSaving = CEP_RTC::DayLightSaving::SubOne; break;
        case RTC_DAYLIGHTSAVING_ADD1H: dayLightSaving = CEP_RTC::DayLightSaving::AddOne; break;
        case RTC_DAYLIGHTSAVING_NONE: [[fallthrough]];
        default: dayLightSaving = CEP_RTC::DayLightSaving::None; break;
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
        case CEP_RTC::DayLightSaving::SubOne: time.DayLightSaving = RTC_DAYLIGHTSAVING_SUB1H; break;
        case CEP_RTC::DayLightSaving::AddOne: time.DayLightSaving = RTC_DAYLIGHTSAVING_ADD1H; break;
        case CEP_RTC::DayLightSaving::None: [[fallthrough]];
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
}    // namespace CEP_RTC


RtcModule* RtcModule::s_instance = nullptr;

RtcModule::RtcModule(RTC_HandleTypeDef* handle, const std::string& label)
: m_label(label), m_handle(handle)
{
    CEP_ASSERT(handle != nullptr, "RTC Handle is NULL!");
    CEP_ASSERT(s_instance == nullptr, "Only one RTC instance is allowed!");

    s_instance = this;

    LOG_INFO("[%s]: Initialized", m_label.c_str());
}

/**
 * For the POST of the RTC to pass, it must validate that the RTC peripheral keeps track of time
 * properly.
 * This is done by first reading the time stored by the RTC. We then wait 2 seconds, then
 * re-read the time stored by the RTC.
 * Since it is really unlikely that the two readings are frame perfect, the second reading should be
 * 1 to 3 seconds after the first reading.
 * @brief   POST function of the RTC module.
 * @return
 */
bool RtcModule::DoPost()
{
    size_t firstTime = GetEpoch();
    HAL_Delay(2000);
    size_t secondTime = GetEpoch();

    if ((secondTime >= firstTime + 1) && (secondTime <= firstTime + 3))
    {
        LOG_INFO("[RTC]: POST OK");
        return true;
    }
    else
    {
        LOG_ERROR("[RTC]: POST ERROR! (first: %i, second: %i)", firstTime, secondTime);
        return false;
    }
}

void RtcModule::Run()
{
}

void RtcModule::SetTime(const CEP_RTC::Time& time)
{
    RTC_TimeTypeDef newTime = time.ToHal();

    if (HAL_RTC_SetTime(m_handle, &newTime, RTC_FORMAT_BIN) != HAL_OK)
    {
        LOG_ERROR("[%s]: Unable to set the time!", m_label.c_str());
    }
}

CEP_RTC::Time RtcModule::GetTime()
{
    RTC_TimeTypeDef halTime = {};

    if (HAL_RTC_GetTime(m_handle, &halTime, RTC_FORMAT_BIN) != HAL_OK)
    {
        LOG_ERROR("[%s]: Unable to get the time!", m_label.c_str());
    }

    // We must always also read the date after reading the time.
    RTC_DateTypeDef dummyDate = {};
    HAL_RTC_GetDate(m_handle, &dummyDate, RTC_FORMAT_BIN);

    return CEP_RTC::Time(halTime);
}

void RtcModule::SetDate(const CEP_RTC::Date& date)
{
    RTC_DateTypeDef newDate = date.ToHal();

    if (HAL_RTC_SetDate(m_handle, &newDate, RTC_FORMAT_BIN) != HAL_OK)
    {
        LOG_ERROR("[%s]: Unable to set the date!", m_label.c_str());
    }
}

CEP_RTC::Date RtcModule::GetDate()
{
    RTC_TimeTypeDef dummyTime = {};

    // We must always read the time before reading the date.
    HAL_RTC_GetTime(m_handle, &dummyTime, RTC_FORMAT_BIN);

    RTC_DateTypeDef halDate = {};
    if (HAL_RTC_GetDate(m_handle, &halDate, RTC_FORMAT_BIN) != HAL_OK)
    {
        LOG_ERROR("[%s]: Unable to get the date!", m_label.c_str());
    }

    return CEP_RTC::Date(halDate);
}

CEP_RTC::Timestamp RtcModule::GetTimestamp()
{
    return CEP_RTC::Timestamp(GetDate(), GetTime());
}

size_t RtcModule::GetEpoch()
{
    return GetEpoch(GetDate(), GetTime());
}

size_t RtcModule::GetEpoch(const CEP_RTC::Date& date, const CEP_RTC::Time& time)
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

CEP_RTC::Timestamp::Timestamp(size_t epoch)
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

size_t CEP_RTC::Timestamp::ToEpoch() const
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

#endif
