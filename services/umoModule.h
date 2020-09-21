/**
 ******************************************************************************
 * @addtogroup umoModule
 * @{
 * @file    umoModule
 * @author  Samuel Martel
 * @brief   Header for the umoModule module.
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
 */
#ifndef _umoModule
#    define _umoModule

/*****************************************************************************/
/* Includes */
#    include "defines/module.hpp"
#    include "drivers/uartModule.hpp"

#    include <array>
#    include <string>
#    include <vector>

/*****************************************************************************/
/* Exported defines */

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
    bool                 isNew    = false;
    std::vector<uint8_t> universe = std::vector<uint8_t>(CHANNEL_COUNT);

    static constexpr size_t CHANNEL_COUNT = 512;
};

class UmoModule : public cep::Module
{
public:
    UmoModule(UartModule* uart, size_t universeCnt, const std::string& label);
    virtual ~UmoModule( ) override = default;

    virtual void               Run( ) override;
    virtual const std::string& GetLabel( ) const override { return m_label; }

    const std::vector<uint8_t>& GetUniverse(size_t universe) const;

    bool IsUniverseReady(size_t universe) const;

    const std::vector<uint8_t> GetChannels(size_t universe, size_t channel, size_t size);

    void SetChannels(size_t universe, size_t channel, const std::vector<uint8_t>& data);
    void SetChannels(size_t universe, size_t channel, uint8_t* data, size_t len);

private:
    UartModule* m_uart  = nullptr;
    std::string m_label = "";

    std::vector<Universe> m_universes;
};

/*****************************************************************************/
/* Exported functions */

/* Have a wonderful day :) */
#endif /* _umoModule */
/**
 * @}
 */
/****** END OF FILE ******/
