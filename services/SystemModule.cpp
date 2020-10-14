/**
 #include <stm32f4xx_hal_cortex.h>
 ******************************************************************************
 * @addtogroup SystemModule
 * @{
 * @file    SystemModule
 * @author  Samuel Martel
 * @brief   Header for the SystemModule module.
 *
 * @date 10/1/2020 11:25:18 AM
 *
 ******************************************************************************
 */
#include "SystemModule.h"
#if defined(NILAI_USE_SYSTEM) && defined(NILAI_USE_UMO)

#    include "stm32f4xx_hal.h"

#    include "services/umoModule.h"

#    include APPLICATION_HEADER

#    define SET_BIT(REG, BIT)   ((REG) |= (BIT))
#    define CLEAR_BIT(REG, BIT) ((REG) &= ~(BIT))
#    define READ_BIT(REG, BIT)  ((REG) & (BIT))

void SystemModule::Run( )
{
    static auto* umo = UMO_MODULE;
    // If we've received a reset request during the last frame:
    if (m_rstRq == 0x01)
    {
        // Reset.
        //        HAL_NVIC_SystemReset( );
    }

    // #TODO Find a more performance friendly way of getting UMO_MODULE.
    if (umo->IsUniverseReady(m_universeId))
    {
        // Check if we should reset.
        if (umo->GetChannels(m_universeId, m_rstChannel, 1)[0] == 1)
        {
            m_rstRq = 0x01;
        }

        // Update status in universe.
        umo->SetChannels(m_universeId,
                         m_statusStartChannel,
                         {(uint8_t)(m_status >> 8), (uint8_t)(m_status & 0x00FF)});

        // Update serial number in universe.
        umo->SetChannels(m_universeId,
                         m_snStartChannel,
                         {(uint8_t)(m_sn >> 8), (uint8_t)(m_sn & 0x00FF)});

        // Update version in universe.
        umo->SetChannels(m_universeId, m_versionChannel, m_versions, sizeof(m_versions));
    }
}

void SystemModule::SetStatus(System::SystemStatus status)
{
    // If status is an error:
    if (status >= System::SystemStatus::HalError && status <= System::SystemStatus::OtherError)
    {
        // Clear the OK bit.
        CLEAR_BIT(m_status, System::SystemStatus::Ok);
    }
    // If status is OK:
    else if (status == System::SystemStatus::Ok)
    {
        // Clear the error bits.
        CLEAR_BIT(m_status,
                  System::SystemStatus::HalError | System::SystemStatus::DriverError |
                      System::SystemStatus::InterfaceError | System::SystemStatus::ServiceError |
                      System::SystemStatus::ProcessError | System::SystemStatus::PostError |
                      System::SystemStatus::OtherError);
    }
    // If status is Standby:
    else if (status == System::SystemStatus::Standby)
    {
        // Clear Busy.
        CLEAR_BIT(m_status, System::SystemStatus::Busy);
    }
    else if (status == System::SystemStatus::Busy)
    {
        // Clear Standby.
        CLEAR_BIT(m_status, System::SystemStatus::Standby);
    }

    SET_BIT(m_status, status);
}
#endif
