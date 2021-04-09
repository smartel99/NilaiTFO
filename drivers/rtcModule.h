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
#ifndef SPI_MODULE_HPP_
#define SPI_MODULE_HPP_
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
namespace CEP_RTC{
struct Time
{
    uint8_t seconds = 0;
    uint8_t minutes = 0;
    uint8_t hours = 0;

    uint8_t day = 0;
    uint8_t dotw = 0; // Day of the week, 0 is Sunday, 6 is Saturday
    uint8_t month = 0; // 0 is January, 11 is December
    uint16_t year = 0;
}

}

/*************************************************************************************************/
/* Classes
 * -------------------------------------------------------------------------------------
 */
class RtcModule : public cep::Module
{
public:
    RtcModule(RTC_HandleTypeDef* handle, const std::string& label)
    : m_label(label), m_handle(handle)
    {
        CEP_ASSERT(handle != nullptr, "RTC Handle is NULL!");
        LOG_INFO("[%s]: Initialized", m_label.c_str());
    }

    virtual ~RtcModule() override = default;

    virtual void               Run() override;
    virtual const std::string& GetLabel() const override { return m_label; }


private:
    std::string        m_label;
    RTC_HandleTypeDef* m_handle;

private:
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
