/**
 * @addtogroup  drivers
 * @{
 * @addtogroup  RTC
 * @{
 * @file        rtcModule
 * @author      Samuel Martel
 * @date        2021/04/09
 *
 * @brief       Real-Time Clock Module
 */
#ifndef RTC_MODULE_HPP_
#define RTC_MODULE_HPP_
/*************************************************************************************************/
/* Includes
 * ------------------------------------------------------------------------------------
 */
#if defined(NILAI_USE_RTC)
#include "defines/internalConfig.h"
#include NILAI_HAL_HEADER
#if defined(HAL_RTC_MODULE_ENABLED)
#include "defines/macros.hpp"
#include "defines/misc.hpp"
#include "defines/module.hpp"

#include <string>
#include <vector>

/*************************************************************************************************/
/* Defines
 * -------------------------------------------------------------------------------------
 */
namespace CEP_RTC
{
enum class DayLightSaving
{
    None   = 0,
    AddOne = 1,
    SubOne = 2
};

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

    RTC_TimeTypeDef ToHal() const;
    std::string     ToStr() const;
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

    std::string DotWtoStr() const;
    std::string MonthtoStr() const;

    RTC_DateTypeDef ToHal() const;
    std::string     ToStr() const;
};

struct Timestamp
{
    Date date;
    Time time;

    Timestamp() = default;
    Timestamp(const Date& d, const Time& t) : date(d), time(t) {}
    Timestamp(size_t epoch);

    size_t ToEpoch() const;

    bool operator>(const Timestamp& other) { return (ToEpoch() > other.ToEpoch()); }
};

}    // namespace CEP_RTC

/*************************************************************************************************/
/* Classes
 * -------------------------------------------------------------------------------------
 */
class RtcModule : public cep::Module
{
public:
    RtcModule(RTC_HandleTypeDef* handle, const std::string& label);

    virtual ~RtcModule() override = default;

    virtual bool               DoPost() override;
    virtual void               Run() override;
    virtual const std::string& GetLabel() const override { return m_label; }

    void          SetTime(const CEP_RTC::Time& time);
    CEP_RTC::Time GetTime();

    void          SetDate(const CEP_RTC::Date& date);
    CEP_RTC::Date GetDate();

    CEP_RTC::Timestamp GetTimestamp();

    size_t        GetEpoch();
    static size_t GetEpoch(const CEP_RTC::Date& date, const CEP_RTC::Time& time);

    static RtcModule* Get() { return s_instance; }

private:
    static RtcModule* s_instance;

private:
    std::string        m_label;
    RTC_HandleTypeDef* m_handle;
};


#else
#if WARN_MISSING_STM_DRIVERS
#warning NilaiTFO RTC module enabled, but HAL_RTC_USE_MODULE is not defined!
#endif
#endif
#endif
#endif
/**
 * @}
 * @}
 */
/* ----- END OF FILE ----- */
