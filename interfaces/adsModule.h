/**
 ******************************************************************************
 * @addtogroup adsModule
 * @{
 * @file    adsModule
 * @author  Samuel Martel
 * @brief   Header for the adsModule module.
 *
 * @date 9/2/2020 8:22:54 AM
 *
 ******************************************************************************
 */
#ifndef _adsModule
#    define _adsModule
#if defined(NILAI_USE_ADS) && defined(NILAI_USE_SPI)
/*****************************************************************************/
/* Includes */
#    include "shared/interfaces/adsModuleConfig.h"

#    include "shared/defines/misc.hpp"
#    include "shared/defines/module.hpp"

#    include "shared/drivers/spiModule.hpp"

#    include <string>

/*****************************************************************************/
/* Exported defines */

/*****************************************************************************/
/* Exported macro */

/*****************************************************************************/
/* Exported types */
struct AdsPacket
{
    uint32_t timestamp = 0;
    float    channel1  = 0.0f;
    float    channel2  = 0.0f;
    float    channel3  = 0.0f;
    float    channel4  = 0.0f;
};

/**
 ** @note   Currently not as universal as it could be.
 **         The interface assumes that:
 **             - SPI mode is always Asynchronous Slave
 **             - Device Words are always 24-bits wide
 **             - Hamming is always off
 **             - Frame Mode is always fixed (6 words long)
 **             - VREF is always 2.442V
 ** */
class AdsModule : public cep::Module
{
public:
    AdsModule(SpiModule* spi, const std::string& label)
        : m_spi(spi), m_label(label), m_config(ADS::Config( ))
    {
    }

    virtual ~AdsModule( ) = default;

    virtual void               Run( ) override;
    virtual const std::string& GetLabel( ) const override { return m_label; }

    const ADS::Config& GetConfig( ) const { return m_config; }

    const AdsPacket& GetLatestPacket( ) const { return m_latestFrame; }

    float GetChannel1( ) const { return m_latestFrame.channel1; }

    float GetChannel2( ) const { return m_latestFrame.channel2; }

    float GetChannel3( ) const { return m_latestFrame.channel3; }

    float GetChannel4( ) const { return m_latestFrame.channel4; }

    // Use force = true if you want the config to be applied no matter what.
    void Configure(const ADS::Config& config = ADS::Config( ), bool force = false);
    void Enable( );
    void Disable( );

    // Using timeout = 0 skips the waiting for DRDY
    const AdsPacket& RefreshValues(uint32_t timeout = 0);

    bool IsActive( ) const { return m_active; }

private:
    bool        m_active = false;
    SpiModule*  m_spi;
    std::string m_label = "";
    ADS::Config m_config;
    AdsPacket   m_latestFrame;

private:
    static const uint8_t MaxInitAttempts = 5;

private:
    inline void     Reset( );
    inline bool     SendCommand(uint16_t cmd, uint16_t expectedResponse);
    inline bool     SendConfig(uint8_t addr, uint8_t data);
    inline uint16_t Send(uint16_t data);
    inline uint16_t ReadCommandResponse( );
    inline int32_t  CalculateTension(uint8_t* data);
    inline float    ConvertToVolt(int16_t val);
};

/*****************************************************************************/
/* Exported functions */

/* Have a wonderful day :) */
#endif /* _adsModule */
#endif
/**
 * @}
 */
/****** END OF FILE ******/
