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
#    if defined(NILAI_USE_ADS) && defined(NILAI_USE_SPI)
/*****************************************************************************/
/* Includes */
#        include "shared/interfaces/adsModuleConfig.h"

#        include "shared/defines/misc.hpp"
#        include "shared/defines/module.hpp"

#        include "shared/drivers/spiModule.hpp"

#        include <deque>
#        include <string>

/*****************************************************************************/
/* Exported defines */

/*****************************************************************************/
/* Exported macro */

/*****************************************************************************/
/* Exported types */
struct AdsPacket
{
    uint32_t timestamp = 0;
    int32_t  channel1  = 0;
    int32_t  channel2  = 0;
    int32_t  channel3  = 0;
    int32_t  channel4  = 0;
};

class SystemModule;

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
    AdsModule(SpiModule* spi, const std::string& label);

    virtual ~AdsModule( ) = default;

    virtual void               Run( ) override;
    virtual const std::string& GetLabel( ) const override { return m_label; }

    const ADS::Config& GetConfig( ) const { return m_config; }

    const AdsPacket& GetLatestPacket( ) const { return m_latestFrame; }

    int32_t GetChannel1( ) const { return m_latestFrame.channel1; }
    int32_t GetChannel2( ) const { return m_latestFrame.channel2; }
    int32_t GetChannel3( ) const { return m_latestFrame.channel3; }
    int32_t GetChannel4( ) const { return m_latestFrame.channel4; }

    // Use force = true if you want the config to be applied no matter what.
    void Configure(const ADS::Config& config = ADS::Config( ), bool force = false);
    void Enable( );
    void Disable( );

    // Using timeout = 0 skips the waiting for DRDY
    const AdsPacket& RefreshValues(uint32_t timeout = 0);

    bool IsActive( ) const { return m_active; }

    void SetSamplesToTake(uint8_t samplesToTake)
    {
        m_samplesToTake = samplesToTake;
        m_channels.resize(samplesToTake);
    }

    uint8_t GetSamplesToTake( ) const { return m_samplesToTake; }

    void ClearBuffers( ) { m_channels.clear( ); }

private:
    bool          m_active = false;
    SpiModule*    m_spi;
    std::string   m_label = "";
    ADS::Config   m_config;
    AdsPacket     m_latestFrame;
    SystemModule* m_sysModule = nullptr;

    uint8_t m_samplesToTake = 1;
    struct
    {
        std::deque<int32_t> channel1;
        std::deque<int32_t> channel2;
        std::deque<int32_t> channel3;
        std::deque<int32_t> channel4;

        /* These methods do not respect CEP's naming convention because
         * they are intended as an interface with std::deque.
         */
        void clear( )
        {
            channel1.clear( );
            channel2.clear( );
            channel3.clear( );
            channel4.clear( );
        }
        void resize(size_t newSize)
        {
            channel1.resize(newSize);
            channel2.resize(newSize);
            channel3.resize(newSize);
            channel4.resize(newSize);
        }
        size_t size( ) const { return channel1.size( ); }
    } m_channels;

private:
    static const uint8_t MaxInitAttempts = 10;

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
#    endif /* _adsModule */
#endif
/**
 * @}
 */
/****** END OF FILE ******/
