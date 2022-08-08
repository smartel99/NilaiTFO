/**
 ******************************************************************************
 * @file    umo_module.h
 * @author  Samuel Martel
 * @brief   Header for the umoModule module.
 *
 * @date 9/17/2020 8:37:46 AM
 *
 * DEFINITIONS:
 * * Universe: A collection of 512 8-bit channels.
 *
 * STEPS:
 * - PC sends Universe to board
 * - Board checks crc
 * - If crc is good:
 *  - Copies Universe
 *  - Set new Universe Ready flag
 *  - Waits a frame for modules to process it
 *      - Modules should check the Universe Ready flag during each frames
 *          - If it is set, the module should read all of its channel
 *            and re-set all of its values in the Universe
 *  - Clear new Universe Ready flag
 *  - Sends the Universe to the PC
 * - If CRC is bad:
 *  - Ignore Universe
 ******************************************************************************
 */

#ifndef GUARD_UMO_MODULE_H
#    define GUARD_UMO_MODULE_H

/*****************************************************************************/
/* Includes */
#    if defined(NILAI_USE_UMO)
#        include "defines/module.hpp"
#        if defined(NILAI_UMO_USE_UART) && defined(NILAI_UMO_USE_CAN)
#            error Cannot use UMO with both CAN and UART!
#        endif
#        if defined(NILAI_UMO_USE_UART)
#            if !defined(NILAI_USE_UART)
#                error Cannot use the UMO module without the UART module!
#            else
#                include "drivers/uartModule.hpp"
#            endif
#        elif defined(NILAI_UMO_USE_CAN)
#            if !defined(NILAI_USE_CAN)
#                error Cannot use the UMO module without the CAN module!
#            else
#                include "drivers/canModule.hpp"
#            endif
#        endif

#        include <array>
#        include <string>
#        include <vector>

/*****************************************************************************/
/* Exported defines */
#        if defined(NILAI_UMO_USE_UART)
using Handle_t = UartModule;
#        elif defined(NILAI_UMO_USE_CAN)
using Handle_t = CanModule;
#        else
#            error You must specify a hardware layer!
using Handle_t = void;
#        endif

/*****************************************************************************/
/* Exported macro */

/*****************************************************************************/
/* Exported types */

/**
 * @struct  Universe
 * @brief   Structure representing a Umo Universe,
 *          containing all of the 512 channels.
 */
struct Universe
{
    //! Number of frames since we received this universe. -1 means we haven't received it.
    int                  age      = -1;
    std::vector<uint8_t> universe = std::vector<uint8_t>(CHANNEL_COUNT);

    static constexpr size_t CHANNEL_COUNT = 512;
};

class UmoModule : public Nilai::Module
{
public:
    UmoModule(Handle_t* handle, size_t universeCnt, const std::string& label);
    virtual ~UmoModule() override = default;

    virtual bool               DoPost() override;
    virtual void               Run() override;
    virtual const std::string& GetLabel() const override { return m_label; }

    const std::vector<uint8_t>& GetUniverse(size_t universe) const;

    bool IsUniverseReady(size_t universe) const;

    std::vector<uint8_t> GetChannels(size_t universe, size_t channel, size_t size);
    void GetChannels(size_t universe, size_t channel, uint8_t* outData, size_t size);

    void SetChannels(size_t universe, size_t channel, const std::vector<uint8_t>& data);
    void SetChannels(size_t universe, size_t channel, uint8_t* data, size_t len);

private:
    Handle_t*   m_handle = nullptr;
    std::string m_label  = "";

    std::vector<Universe> m_universes;

    static constexpr int OLDEST_AGE = 650;    // Number of frames that a universe can live for.
};

/*****************************************************************************/
/* Exported functions */
#    endif
/* Have a wonderful day :) */
#endif /* GUARD_UMO_MODULE_H */
/****** END OF FILE ******/
