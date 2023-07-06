/**
 ******************************************************************************
 * @file    ads131_module.h
 * @author  Samuel Martel
 * @brief   Header for the adsModule module.
 *
 * @date 9/2/2020 8:22:54 AM
 *
 ******************************************************************************
 */
#ifndef GUARD_ADS131_MODULE_H
#    define GUARD_ADS131_MODULE_H

#    if defined(NILAI_USE_ADS) && defined(NILAI_USE_SPI)
/*****************************************************************************/
/* Includes */
#        include "ads131_module_config.h"

#        include "../../defines/misc.h"
#        include "../../defines/module.h"

#        include "../../drivers/spi_module.h"

#        include <string>
#        include <vector>

namespace Nilai::Interfaces
{
/*****************************************************************************/
/* Exported defines */

/*****************************************************************************/
/* Exported macro */

/*****************************************************************************/
/* Exported types */

/**
 * @enum TrigEdge
 * @brief Edge on which to trigger the sampling.
 */
enum class TrigEdge
{
    Alter   = 0,    //!< Voltage goes above or bellow trigger level.
    Rising  = 1,    //!< Voltage rises above trigger level.
    Falling = 2,    //!< Voltage falls bellow trigger level.
};

/**
 * @struct AdsPacket
 * @brief Contains the last sampling results from the ADC.
 */
struct AdsPacket
{
    //! Time at which the packet was created.
    uint32_t timestamp = 0;

    //! Average value read on channel 1.
    float avgChannel1 = 0;
    //! Average value read on channel 2.
    float avgChannel2 = 0;
    //! Average value read on channel 3.
    float avgChannel3 = 0;
    //! Average value read on channel 4.
    float avgChannel4 = 0;

    //! Smallest value read on channel 1.
    float minChannel1 = 0;
    //! Smallest value read on channel 2.
    float minChannel2 = 0;
    //! Smallest value read on channel 3.
    float minChannel3 = 0;
    //! Smallest value read on channel 4.
    float minChannel4 = 0;

    //! Highest value read on channel 1.
    float maxChannel1 = 0;
    //! Highest value read on channel 2.
    float maxChannel2 = 0;
    //! Highest value read on channel 3.
    float maxChannel3 = 0;
    //! Highest value read on channel 4.
    float maxChannel4 = 0;

    //! RMS value read on channel 1.
    float rmsChannel1 = 0;
    //! RMS value read on channel 2.
    float rmsChannel2 = 0;
    //! RMS value read on channel 3.
    float rmsChannel3 = 0;
    //! RMS value read on channel 4.
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
class AdsModule : public Nilai::Module
{
public:
    AdsModule(Drivers::SpiModule* spi, std::string label);
    ~AdsModule() override = default;

    [[nodiscard]] bool               DoPost() override;
    void                             Run() override;
    [[nodiscard]] const std::string& GetLabel() const { return m_label; }

    [[nodiscard]] const ADS::Config& GetConfig() const { return m_config; }

    [[nodiscard]] const AdsPacket& GetLatestPacket() const { return m_latestFrame; }

    [[nodiscard]] const std::vector<float>& GetChannel(size_t channel) const;

    [[nodiscard]] float GetAverageChannel1() const { return m_latestFrame.avgChannel1; }
    [[nodiscard]] float GetAverageChannel2() const { return m_latestFrame.avgChannel2; }
    [[nodiscard]] float GetAverageChannel3() const { return m_latestFrame.avgChannel3; }
    [[nodiscard]] float GetAverageChannel4() const { return m_latestFrame.avgChannel4; }

    [[nodiscard]] float GetMinChannel1() const { return m_latestFrame.minChannel1; }
    [[nodiscard]] float GetMinChannel2() const { return m_latestFrame.minChannel2; }
    [[nodiscard]] float GetMinChannel3() const { return m_latestFrame.minChannel3; }
    [[nodiscard]] float GetMinChannel4() const { return m_latestFrame.minChannel4; }

    [[nodiscard]] float GetMaxChannel1() const { return m_latestFrame.maxChannel1; }
    [[nodiscard]] float GetMaxChannel2() const { return m_latestFrame.maxChannel2; }
    [[nodiscard]] float GetMaxChannel3() const { return m_latestFrame.maxChannel3; }
    [[nodiscard]] float GetMaxChannel4() const { return m_latestFrame.maxChannel4; }

    [[nodiscard]] float GetRmsChannel1() const { return m_latestFrame.rmsChannel1; }
    [[nodiscard]] float GetRmsChannel2() const { return m_latestFrame.rmsChannel2; }
    [[nodiscard]] float GetRmsChannel3() const { return m_latestFrame.rmsChannel3; }
    [[nodiscard]] float GetRmsChannel4() const { return m_latestFrame.rmsChannel4; }

    // Use force = true if you want the config to be applied no matter what.
    void Configure(const ADS::Config& config = ADS::Config(), bool force = false);
    void Enable();
    void Disable();

    // Using timeout = 0 skips the waiting for DRDY
    const AdsPacket& RefreshValues(uint32_t timeout = 0);
    inline float     CalculateTension(uint8_t* data);

    [[nodiscard]] bool IsActive() const { return m_active; }

    void SetSamplesToTake(uint16_t samplesToTake, bool repeat = true, uint16_t samplesToIgnore = 0)
    {
        m_repeat          = repeat;
        m_samplesToTake   = samplesToTake;
        m_samplesToIgnore = samplesToIgnore;
        m_channels.resize(samplesToTake);
    }
    [[nodiscard]] uint8_t GetSamplesToTake() const { return m_samplesToTake; }

    void SetCallback(const std::function<void(const AdsPacket&)>& cb) { m_callback = cb; }

    void SetTriggerChannel(uint8_t ch) { m_trigChannel = ch; }
    void SetTriggerLevel(float level) { m_trigLevel = level; }
    void SetTriggerEdge(TrigEdge edge) { m_trigEdge = edge; }

    void ClearBuffers() { m_channels.clear(); }

private:
    bool                m_active = false;
    Drivers::SpiModule* m_spi;
    std::string         m_label;
    ADS::Config         m_config;
    bool                m_isConfigured = false;
    AdsPacket           m_latestFrame;
    uint32_t            m_lastStartTime = 0;

    uint8_t  m_trigChannel  = 0;
    float    m_trigLevel    = 0.0f;
    TrigEdge m_trigEdge     = TrigEdge::Alter;
    bool     m_hasTriggered = false;

    uint16_t                              m_samplesToTake   = 1;
    uint16_t                              m_samplesToIgnore = 0;
    uint16_t                              m_samplesTaken    = 0;
    std::function<void(const AdsPacket&)> m_callback;
    bool                                  m_repeat = true;
    struct ChannelData
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

private:
    static const uint8_t MaxInitAttempts = 10;

private:
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
}    // namespace Nilai::Interfaces
/* Have a wonderful day :) */
#    endif /* _adsModule */
#endif
/****** END OF FILE ******/
