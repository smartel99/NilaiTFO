/**
 ******************************************************************************
 * @file    AdcModule.h
 * @author  Samuel Martel
 * @brief
 *
 * @date 2021-11-29
 *
 ******************************************************************************
 */
#ifndef GUARD_ADCMODULE_H
#define GUARD_ADCMODULE_H

#include <functional>
#include <utility>
#include <vector>

#include "defines/module.h"

class AdcModule : Nilai::Module
{
public:
    AdcModule(void* adc, std::string label) : m_label(std::move(label)) {}

    bool                             DoPost() override { return true; }
    void                             Run() override {}
    [[nodiscard]] const std::string& GetLabel() const { return m_label; }

    void AddConvCpltCallback(const std::string& name, const std::function<void(AdcModule*)>& cb)
    {
        cb(this);
    }
    [[maybe_unused]] void AddErrorCallback(const std::string&                     name,
                                           const std::function<void(AdcModule*)>& cb)
    {
        cb(this);
    }

    void Start() { m_started = true; }

    void Stop() { m_started = false; }

    void                SetChannelCount(size_t cnt) { m_channelBuff.resize(cnt); }
    [[nodiscard]] float GetChannelReading(size_t channel) const
    {
        if (m_started)
        {
            return m_channelBuff.at(channel);
        }
        else
        {
            return 0.0f;
        }
    }

    void SetChannelReading(size_t channel, float v) { m_channelBuff.at(channel) = v; }

private:
    std::string m_label;

    bool               m_started     = false;
    std::vector<float> m_channelBuff = {};
};



#endif    // GUARD_ADCMODULE_H
