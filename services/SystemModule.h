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
#ifndef _SystemModule
#    define _SystemModule

/*****************************************************************************/
/* Includes */
#    if defined(NILAI_USE_SYSTEM)
#        if !defined(NILAI_USE_UMO)
#            error Cannot use System module without UMO!
#        else
#            include "defines/module.hpp"
#            include "version.h"

/*****************************************************************************/
/* Exported defines */

/*****************************************************************************/
/* Exported macro */

/*****************************************************************************/
/* Exported types */
namespace System
{
enum SystemStatus
{
    // Error type:
    Ok             = 0x0001,
    HalError       = 0x0002,
    DriverError    = 0x0004,
    InterfaceError = 0x0008,
    ServiceError   = 0x0010,
    ProcessError   = 0x0020,
    PostError      = 0x0040,
    OtherError     = 0x0080,

    // Status type:
    Standby = 0x0100,
    Busy    = 0x0200,

    // Fixture ID:
    IdBit1 = 0x1000,
    IdBit2 = 0x2000,
    IdBit3 = 0x4000,

    // Physical Status:
    LidOpen = 0x8000,
};
}

class SystemModule : public cep::Module
{
public:
    SystemModule(const std::string& label,
                 uint8_t            universe           = 0,
                 uint8_t            rstChannel         = 0,
                 uint8_t            statusStartChannel = 1,
                 uint8_t            snStartChannel     = 2,
                 uint8_t            versionChannel     = 3);
    virtual ~SystemModule( ) = default;

    virtual void               Run( ) override;
    virtual const std::string& GetLabel( ) const { return m_label; }

    void SetStatus(System::SystemStatus status);

    System::SystemStatus GetStatus( ) const { return (System::SystemStatus)m_status; }

    uint16_t GetSn( ) const { return m_sn; }

    void SetSn(uint16_t sn = 0x0000) { m_sn = sn; }

private:
    std::string m_label       = "";
    uint16_t    m_status      = 0x0001;
    uint16_t    m_fixtureId   = 0x0000;
    uint16_t    m_sn          = 0x0000;
    uint8_t     m_versions[6] = {NILAI_VERSION_MAJOR,
                             NILAI_VERSION_MINOR,
                             NILAI_VERSION_BUILD,
                             FW_VERSION_MAJOR,
                             FW_VERSION_MINOR,
                             FW_VERSION_BUILD};

    uint8_t m_rstRq              = 0x00;
    uint8_t m_universeId         = 0x00;
    uint8_t m_rstChannel         = 0x00;
    uint8_t m_snStartChannel     = 0x01;
    uint8_t m_statusStartChannel = 0x02;
    uint8_t m_versionChannel     = 0x03;
};

/*****************************************************************************/
/* Exported functions */
#        endif
#    endif
/* Have a wonderful day :) */
#endif /* _SystemModule */
/**
 * @}
 */
/****** END OF FILE ******/
