#include "adcModule.hpp"
#if defined(NILAI_USE_ADC) && defined(HAL_ADC_MODULE_ENABLED)
#include "services/logger.hpp"
#include "defines/macros.hpp"

static constexpr float ConvertToVolt(uint32_t val)
{
    return (((float)val / 4095.0f) * 3.3f);
}

AdcModule::AdcModule(ADC_HandleTypeDef* adc, const std::string& label) : m_adc(adc), m_label(label)
{
    CEP_ASSERT(adc != nullptr, "[%s]: ADC handle is null!", m_label.c_str());
    m_channelCount = adc->Init.NbrOfConversion;
    m_channelBuff  = new uint32_t[m_channelCount];

    CEP_ASSERT(m_channelBuff != nullptr,
               "[%s]: Unable to allocate memory for channel data buffer!",
               m_label.c_str());
    HAL_ADC_Start_DMA(&hadc1, &m_channelBuff[0], m_channelCount);

    LOG_INFO("[%s]: Initialized", m_label.c_str());
}

AdcModule::~AdcModule()
{
    delete[] m_channelBuff;
    m_channelBuff = nullptr;
}

float AdcModule::GetChannelReading(size_t channel) const
{
    CEP_ASSERT(channel < m_channelCount,
               "[%s] Channel %i is not a valid channel!",
               m_label.c_str(),
               channel);

    return ConvertToVolt(m_channelBuff[channel]);
}

#endif
