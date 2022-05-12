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

#        include <queue>
#        include <string>
#        include <vector>

/*****************************************************************************/
/* Exported defines */

/*****************************************************************************/
/* Exported macro */

/*****************************************************************************/
/* Exported types */
enum class TrigEdge
{
    Alter   = 0,    // Voltage goes above or bellow trigger level.
    Rising  = 1,    // Voltage rises above trigger level.
    Falling = 2,    // Voltage falls bellow trigger level.
};

struct AdsPacket
{
    uint32_t timestamp = 0;

    float avgChannel1 = 0;
    float avgChannel2 = 0;
    float avgChannel3 = 0;
    float avgChannel4 = 0;

    float minChannel1 = 0;
    float minChannel2 = 0;
    float minChannel3 = 0;
    float minChannel4 = 0;

    float maxChannel1 = 0;
    float maxChannel2 = 0;
    float maxChannel3 = 0;
    float maxChannel4 = 0;

    float rmsChannel1 = 0;
    float rmsChannel2 = 0;
    float rmsChannel3 = 0;
    float rmsChannel4 = 0;
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
    virtual ~AdsModule() = default;

    virtual bool               DoPost() override;
    virtual void               Run() override;
    virtual const std::string& GetLabel() const override { return m_label; }

    const ADS::Config& GetConfig() const { return m_config; }

    const AdsPacket& GetLatestPacket() const { return m_latestFrame; }

    const std::vector<float>& GetChannel(size_t channel) const;

    float GetAverageChannel1() const { return m_latestFrame.avgChannel1; }
    float GetAverageChannel2() const { return m_latestFrame.avgChannel2; }
    float GetAverageChannel3() const { return m_latestFrame.avgChannel3; }
    float GetAverageChannel4() const { return m_latestFrame.avgChannel4; }

    float GetMinChannel1() const { return m_latestFrame.minChannel1; }
    float GetMinChannel2() const { return m_latestFrame.minChannel2; }
    float GetMinChannel3() const { return m_latestFrame.minChannel3; }
    float GetMinChannel4() const { return m_latestFrame.minChannel4; }

    float GetMaxChannel1() const { return m_latestFrame.maxChannel1; }
    float GetMaxChannel2() const { return m_latestFrame.maxChannel2; }
    float GetMaxChannel3() const { return m_latestFrame.maxChannel3; }
    float GetMaxChannel4() const { return m_latestFrame.maxChannel4; }

    float GetRmsChannel1() const { return m_latestFrame.rmsChannel1; }
    float GetRmsChannel2() const { return m_latestFrame.rmsChannel2; }
    float GetRmsChannel3() const { return m_latestFrame.rmsChannel3; }
    float GetRmsChannel4() const { return m_latestFrame.rmsChannel4; }

    // Use force = true if you want the config to be applied no matter what.
    void Configure(const ADS::Config& config = ADS::Config(), bool force = false);
    void Enable();
    void Disable();

    // Using timeout = 0 skips the waiting for DRDY
    const AdsPacket& RefreshValues(uint32_t timeout = 0);
    inline float     CalculateTension(uint8_t* data);

    bool IsActive() const { return m_active; }

    void SetSamplesToTake(uint16_t samplesToTake, bool repeat = true, uint16_t samplesToIgnore = 0)
    {
        m_repeat          = repeat;
        m_samplesToTake   = samplesToTake;
        m_samplesToIgnore = samplesToIgnore;
        m_channels.resize(samplesToTake);
    }
    uint8_t GetSamplesToTake() const { return m_samplesToTake; }

    void SetCallback(const std::function<void(const AdsPacket&)>& cb) { m_callback = cb; }

    void SetTriggerChannel(uint8_t ch) { m_trigChannel = ch; }
    void SetTriggerLevel(float level) { m_trigLevel = level; }
    void SetTriggerEdge(TrigEdge edge) { m_trigEdge = edge; }

    void ClearBuffers() { m_channels.clear(); }

    bool        m_active = false;
    SpiModule*  m_spi;
    std::string m_label = "";
    ADS::Config m_config;
    bool        m_isConfigured = false;
    AdsPacket   m_latestFrame;
    uint32_t    m_lastStartTime = 0;

    std::queue<std::array<uint8_t,16>> m_dataRx;

    uint8_t  m_trigChannel  = 0;
    float    m_trigLevel    = 0.0f;
    TrigEdge m_trigEdge     = TrigEdge::Alter;
    bool     m_hasTriggered = false;

    uint16_t                              m_samplesToTake   = 1;
    uint16_t                              m_samplesToIgnore = 0;
    uint16_t                              m_samplesTaken    = 0;
    std::function<void(const AdsPacket&)> m_callback;
    bool                                  m_repeat = true;
    struct
    {
        std::vector<float> channel1 = std::vector<float>(1024, 0.0f);
        std::vector<float> channel2 = std::vector<float>(1024, 0.0f);
        std::vector<float> channel3 = std::vector<float>(1024, 0.0f);
        std::vector<float> channel4 = std::vector<float>(1024, 0.0f);

        /* These methods do not respect CEP's naming convention because
         * they are intended as an interface with std::vector.
         */
        void clear()
        {
            channel1.clear();
            channel2.clear();
            channel3.clear();
            channel4.clear();
        }
        void resize(size_t newSize)
        {
            channel1.resize(newSize);
            channel2.resize(newSize);
            channel3.resize(newSize);
            channel4.resize(newSize);
        }
        size_t              size() const { return channel1.size(); }
        std::vector<float>& operator[](std::size_t idx)
        {
            switch (idx)
            {
                case 0: return channel1;
                case 1: return channel2;
                case 2: return channel3;
                case 3: return channel4;
                default: return channel1;
            }
        }
        const std::vector<float>& operator[](std::size_t idx) const
        {
            switch (idx)
            {
                case 0: return channel1;
                case 1: return channel2;
                case 2: return channel3;
                case 3: return channel4;
                default: return channel1;
            }
        }
    } m_channels;

    static const uint8_t MaxInitAttempts = 10;

    inline void     Reset();
    inline bool     SendCommand(uint16_t cmd, uint16_t expectedResponse);
    inline bool     SendConfig(uint8_t addr, uint8_t data);
    inline uint16_t Send(uint16_t data);
    inline uint16_t ReadCommandResponse();
    inline float    ConvertToVolt(int32_t val);
    inline uint32_t ConvertToHex(float val);
    void            UpdateLatestFrame();
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
