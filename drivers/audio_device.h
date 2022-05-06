/**
 * @file    audio_device.h
 * @author  Samuel Martel
 * @date    2022-05-04
 * @brief
 *
 * @copyright
 * This program is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If
 * not, see <a href=https://www.gnu.org/licenses/>https://www.gnu.org/licenses/<a/>.
 */
#ifndef NILAI_AUDIO_DEVICE_H
#define NILAI_AUDIO_DEVICE_H

#if defined(NILAI_USE_I2S) || defined(NILAI_USE_SAI)
#    include "../defines/module.h"

#    include <functional>

namespace Nilai::Drivers
{
class AudioDevice : public Module
{
public:
    ~AudioDevice() override = default;

    virtual void StartClock() {}
    virtual void StopClock() {}

    virtual void SetTxHalfCpltCb([[maybe_unused]] const std::function<void()>& cb) {}
    virtual void SetTxCpltCb([[maybe_unused]] const std::function<void()>& cb) {}

    [[nodiscard]] virtual bool IsStreaming() const { return false; }
    /**
     * @brief Streams I2S data using DMA.
     * @param samples The samples to send. Must be valid until the DMA transfer is completed.
     * @param cnt The number of samples to send, combining L+R channels.
     * @return True if the stream was successfully started.
     */
    virtual bool Stream([[maybe_unused]] const void* samples, [[maybe_unused]] size_t cnt)
    {
        return false;
    }
    virtual bool Restream([[maybe_unused]] const void* samples, [[maybe_unused]] size_t cnt)
    {
        return false;
    }

    /**
     * @brief Pauses an ongoing stream.
     * @return True if successfully paused.
     */
    virtual bool PauseStream() { return false; }

    /**
     * @brief Resumes an ongoing stream.
     * @return True if successfully resumed.
     */
    virtual bool ResumeStream() { return false; }

    /**
     * @brief Stops an ongoing stream.
     * @return True if successfully stopped.
     */
    virtual bool StopStream() { return false; }

    virtual void ToggleMute([[maybe_unused]] bool s) {}

    virtual void               ToggleSleep([[maybe_unused]] bool s) {}
    [[nodiscard]] virtual bool IsAsleep() const { return false; }

    virtual void               ToggleShutdown([[maybe_unused]] bool s) {}
    [[nodiscard]] virtual bool IsShutdown() const { return false; }
};

}    // namespace Nilai::Drivers
#endif
#endif    // NILAI_AUDIO_DEVICE_H
