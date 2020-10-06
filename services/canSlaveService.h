/**
 ******************************************************************************
 * @addtogroup canSlaveService
 * @{
 * @file    canSlaveService
 * @author  Samuel Martel
 * @brief   Header for the canSlaveService module.
 *
 * @date 9/17/2020 8:37:46 AM
 *
 ******************************************************************************
 */

/**
 * DEFINITION:
 * * Universe: A collection of [TBD] 8-bit channels.
 *
 * STEPS:
 * - MASTER board sends one or more channels
 * - Slave checks crc
 * - If crc is good:
 *  - Copies channels into local universe
 *  - Set new Universe Ready flag ??????
 *  - Waits a frame for modules to process it
 *      - Modules should check the Universe Ready flag during each frames
 *          - If it is set, the module should read all of its channel
 *            and re-set all of its values in the Universe
 *  - Clear new Universe Ready flag
 *  - Sends the Universe to the PC
 * - If CRC is bad:
 *  - Ignore Universe
 */
#ifndef _canSlaveModule
#    define _canSlaveModule

/*****************************************************************************/
/* Includes */
#    include "defines/module.hpp"

#    include "drivers/canModule.hpp"

#    include <array>
#    include <string>
#    include <vector>

/*****************************************************************************/
/* Exported defines */

/*****************************************************************************/
/* Exported macro */

/*****************************************************************************/
/* Exported types */

class CanSlaveModule: public cep::Module
{
public:
    CanSlaveModule(CanModule *can2, const std::string &label);
    virtual ~CanSlaveModule() override = default;

    virtual void Run() override;
    virtual const std::string& GetLabel() const override
    {
        return m_label;
    }

private:
    CanModule *m_can2 = nullptr;
    std::string m_label = "";

};

/*****************************************************************************/
/* Exported functions */

/* Have a wonderful day :) */
#endif /* _canSlaveModule */
/**
 * @}
 */
/****** END OF FILE ******/
