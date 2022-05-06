/**
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
#include "system_module.h"
#if defined(NILAI_USE_SYSTEM) && defined(NILAI_USE_UMO)

#    include NILAI_HAL_HEADER

#    include "services/umoModule.h"

#    include APPLICATION_HEADER

#    define SET_BIT(REG, BIT)   ((REG) |= (BIT))
#    define CLEAR_BIT(REG, BIT) ((REG) &= ~(BIT))
#    define READ_BIT(REG, BIT)  ((REG) & (BIT))

SystemModule::SystemModule(const std::string& label,
                           uint8_t            universe,
                           uint8_t            rstChannel,
                           uint8_t            statusStartChannel,
                           uint8_t            snStartChannel,
                           uint8_t            versionChannel)
: m_label(label),
  m_universeId(universe),
  m_rstChannel(rstChannel),
  m_snStartChannel(snStartChannel),
  m_statusStartChannel(statusStartChannel),
  m_versionChannel(versionChannel)
{
    HAL_GPIO_ReadPin(fixtureID1_GPIO_Port, fixtureID1_Pin) == GPIO_PIN_RESET
      ? SET_BIT(m_fixtureId, System::IdBit1)
      : CLEAR_BIT(m_fixtureId, System::IdBit1);
    HAL_GPIO_ReadPin(fixtureID2_GPIO_Port, fixtureID2_Pin) == GPIO_PIN_RESET
      ? SET_BIT(m_fixtureId, System::IdBit2)
      : CLEAR_BIT(m_fixtureId, System::IdBit2);
    HAL_GPIO_ReadPin(fixtureID3_GPIO_Port, fixtureID3_Pin) == GPIO_PIN_RESET
      ? SET_BIT(m_fixtureId, System::IdBit3)
      : CLEAR_BIT(m_fixtureId, System::IdBit3);

    m_status |= m_fixtureId;
}

bool SystemModule::DoPost()
{
#    warning No POSTs have been written for this module!
    return false;
}

void SystemModule::Run()
{
    static auto* umo = UMO_MODULE;

    m_status |= m_fixtureId;
    // Check if lid is open or closed.
    // COVER_IN == high -> lid open
    m_status =
      (HAL_GPIO_ReadPin(COVER_IN_GPIO_Port, COVER_IN_Pin)) ? m_status | 0x8000 : m_status & 0x7FFF;

    // If we've received a reset request during the last frame:
    if (m_rstRq == 0x01)
    {
        // Reset.
        //        HAL_NVIC_SystemReset( );
    }

    if (umo->IsUniverseReady(m_universeId))
    {
        // Check if we should reset. 97
        if (umo->GetChannels(m_universeId, m_rstChannel, 1)[0] == 1)
        {
            m_rstRq = 0x01;
        }

        // Update status in universe. 100
        umo->SetChannels(m_universeId,
                         m_statusStartChannel,
                         {(uint8_t)(m_status >> 8), (uint8_t)(m_status & 0x00FF)});

        // Update serial number in universe. 102
        umo->SetChannels(
          m_universeId, m_snStartChannel, {(uint8_t)(m_sn >> 8), (uint8_t)(m_sn & 0x00FF)});

        // Update version in universe. 130
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
