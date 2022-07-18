/**
 * @file    at24qt2120.cpp
 * @author  Samuel Martel
 * @date    2022-07-12
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
#include "at24qt2120.h"

#if defined(NILAI_USE_AT24QT2120)

#    include "../../processes/application.h"

#    include "registers/registers.h"

#    include <algorithm>

#    define TS_ENABLE_DEBUG

#    if defined(TS_ENABLE_DEBUG)
#        define TS_DEBUG(msg, ...) LOG_DEBUG("[AT24QT]: " msg, ##__VA_ARGS__)
#        define TS_VERIFY_VALUE(exp, v)                                                            \
            do                                                                                     \
            {                                                                                      \
                auto readback = v;                                                                 \
                NILAI_ASSERT(                                                                      \
                  (exp) == readback, "Readback failed, exp: %i, got: %i", exp, readback);          \
            } while (0)
#    else
#        define TS_DEBUG(msg, ...)
#        define TS_VERIFY_VALUE(x, msg, ...)
#    endif
#    define TS_INFO(msg, ...)  LOG_INFO("[AT24QT]: " msg, ##__VA_ARGS__)
#    define TS_ERROR(msg, ...) LOG_ERROR("[AT24QT]: " msg, ##__VA_ARGS__)

#    define IS_NOT_DEFAULT(v) (m_values.v != defaults.v)
#    define CONFIG_IF_NOT_DEFAULT(v)                                                               \
        do                                                                                         \
        {                                                                                          \
            if (IS_NOT_DEFAULT(v))                                                                 \
            {                                                                                      \
                obj.Set##v(m_values.v);                                                            \
            }                                                                                      \
        } while (0)

namespace Nilai::Interfaces
{
using Registers  = AT24QT2120::Registers;
using KeyOptions = AT24QT2120::KeyOptions;
using namespace std::string_view_literals;

At24Qt2120 At24Qt2120::Builder::Build() const
{
    using FirmwareVersion = AT24QT2120::FirmwareVersion;

    At24Qt2120 obj {m_i2c};

    // Trigger a full chip reset.
    obj.Reset();

    // Verify that the chip is there.
    const uint8_t id = obj.GetId();
    NILAI_ASSERT(id == At24Qt2120::s_chipId,
                 "Expected chip ID to be %#02x, read %#02x",
                 At24Qt2120::s_chipId,
                 id);

    // Verify the firmware version.
    FirmwareVersion version = obj.GetFirmwareVersion();
    TS_INFO("FW: v%i.%i", version.Major, version.Minor);

    AT24QT2120::RegisterMap defaults = {};
    if (IS_NOT_DEFAULT(Calibrate))
    {
        obj.Calibrate(false);
    }

    if (IS_NOT_DEFAULT(Reset))
    {
        obj.Reset();
    }

    CONFIG_IF_NOT_DEFAULT(SamplingInterval);
    CONFIG_IF_NOT_DEFAULT(TowardTouchDrift);
    CONFIG_IF_NOT_DEFAULT(AwayFromTouchDrift);
    CONFIG_IF_NOT_DEFAULT(DetectionIntegrator);
    CONFIG_IF_NOT_DEFAULT(TouchRecalibrationDelay);
    CONFIG_IF_NOT_DEFAULT(DriftHoldTime);

    if (IS_NOT_DEFAULT(SliderOption))
    {
        obj.SetSliderOptions(m_values.SliderOption);
    }

    CONFIG_IF_NOT_DEFAULT(ChargeTime);

    size_t                at         = 0;
    AT24QT2120::KeyValues defaultKey = {};
    for (const auto& key : m_values.Keys)
    {
        AT24QT2120::Keys atKey = static_cast<AT24QT2120::Keys>(at);
        if (key.DetectThreshold != defaultKey.DetectThreshold)
        {
            obj.SetDetectionThreshold(atKey, key.DetectThreshold);
        }

        if (key.Configuration.Enable != defaultKey.Configuration.Enable)
        {
            obj.KeyIsOutput(atKey, key.Configuration.Enable);
        }

        if (key.Configuration.GPO != defaultKey.Configuration.GPO)
        {
            obj.SetKeyState(atKey, key.Configuration.GPO);
        }

        if (key.Configuration.Guard != defaultKey.Configuration.Guard)
        {
            obj.KeyIsGuard(atKey, key.Configuration.Guard);
        }

        if (key.Configuration.AKS != defaultKey.Configuration.AKS)
        {
            obj.KeyInGroup(atKey, static_cast<AT24QT2120::Group>(key.Configuration.AKS));
        }

        if (key.PulseAndScale.Pulse != defaultKey.PulseAndScale.Pulse)
        {
            obj.SetKeyPulse(atKey, key.PulseAndScale.Pulse);
        }

        if (key.PulseAndScale.Scale != defaultKey.PulseAndScale.Scale)
        {
            obj.SetKeyScale(atKey, key.PulseAndScale.Scale);
        }

        at++;
    }

#    if defined(NILAI_USE_EVENTS)
    if (m_isInIrqMode)
    {
        obj.m_run = &At24Qt2120::IrqRun;
        obj.BindIrq(m_irqType);
    }
    else
#    endif
    {
        obj.m_run       = &At24Qt2120::PollingRun;
        obj.m_changePin = m_pin;
    }

    obj.Calibrate();

    return obj;
}

bool At24Qt2120::DoPost()
{
    bool passed = I2cModule::DoPost();

    // TODO Write AT24QT2120 POST.
    TS_INFO("POST OK");

    return passed;
}

void At24Qt2120::Run()
{
    m_run(this);
}

uint32_t At24Qt2120::GetLastEventTime() const
{
    return m_lastEventTime;
}

void At24Qt2120::PollingRun(At24Qt2120* self)
{
    static bool lastState = false;

    bool currentState = self->m_changePin.Get();

    if (currentState != lastState)
    {
        lastState = currentState;
        // Pin goes low when an event occurs.
        if (!currentState)
        {
            self->m_lastEventTime = Nilai::GetTime();
        }
    }
}

#    if defined(NILAI_USE_EVENTS)
void At24Qt2120::IrqRun([[maybe_unused]] At24Qt2120* self)
{
    // Nothing to do here...
}

bool At24Qt2120::HandleIrq(Events::Event* e)
{
    if (e->Type == m_irqType)
    {
        // Event triggered by the touch sensor, record the timestamp.
        m_lastEventTime = e->Timestamp;
        return true;
    }

    // Not our event.
    return false;
}

void At24Qt2120::BindIrq(Events::EventTypes type)
{
    m_irqType = type;
    m_irqId   = Application::Get()->RegisterEventCallback(
      type, [this](Events::Event* e) { return HandleIrq(e); });
}


uint8_t At24Qt2120::GetId() noexcept
{
    static constexpr uint8_t REG_SIZE = 1;

    I2C::Frame f =
      ReceiveFrameFromRegister(s_i2cAddress, static_cast<uint8_t>(Registers::ChipId), REG_SIZE);

    return f.data.front();
}

AT24QT2120::FirmwareVersion At24Qt2120::GetFirmwareVersion() noexcept
{
    static constexpr uint8_t REG_SIZE = 1;

    I2C::Frame f = ReceiveFrameFromRegister(
      s_i2cAddress, static_cast<uint8_t>(Registers::FirmwareVersion), REG_SIZE);

    return AT24QT2120::FirmwareVersion {f.data.front()};
}

AT24QT2120::DetectionStatus At24Qt2120::GetDetectionStatus() noexcept
{
    static constexpr uint8_t REG_SIZE = 1;

    I2C::Frame f = ReceiveFrameFromRegister(
      s_i2cAddress, static_cast<uint8_t>(Registers::DetectionStatus), REG_SIZE);

    return AT24QT2120::DetectionStatus {f.data.front()};
}

AT24QT2120::KeyStatus At24Qt2120::GetKeyStatus() noexcept
{
    static constexpr uint8_t REG_SIZE = 2;

    I2C::Frame f =
      ReceiveFrameFromRegister(s_i2cAddress, static_cast<uint8_t>(Registers::KeyStatus1), REG_SIZE);

    uint16_t keyStatus = (static_cast<uint16_t>(f.data[0]) << 8) | static_cast<uint16_t>(f.data[1]);
    return AT24QT2120::KeyStatus {keyStatus};
}

uint8_t At24Qt2120::GetSliderPosition() noexcept
{
    static constexpr uint8_t REG_SIZE = 1;

    I2C::Frame f = ReceiveFrameFromRegister(
      s_i2cAddress, static_cast<uint8_t>(Registers::SliderPosition), REG_SIZE);

    return f.data.front();
}

bool At24Qt2120::Calibrate(bool waitForEnd) noexcept
{
    TS_DEBUG("Started calibration...");

    uint32_t startTime = Nilai::GetTime();

    // Writing anything other than 0 to the calibration register initiates a calibration cycle.
    const uint8_t startCalibration = 0xFF;
    TransmitFrameToRegister(s_i2cAddress,
                            static_cast<uint8_t>(Registers::Calibrate),
                            &startCalibration,
                            sizeof(startCalibration));

    if (waitForEnd)
    {
        // Datasheet says that a calibration cycle takes 15 measurements at LPM = 1.
        // 15 cycles * 16ms = 240ms.
        // We wait slightly less than that, in case the sensor finishes before us.
        static constexpr uint32_t CALIBRATION_WAIT_TIME = 230;
        Nilai::Delay(CALIBRATION_WAIT_TIME);

        // Give the sensor between 230 and 250ms to calibrate itself.
        static constexpr uint32_t MAX_VERIFICATION_TIME = 20;
        uint32_t                  maxTime               = Nilai::GetTime() + MAX_VERIFICATION_TIME;

        AT24QT2120::DetectionStatus status = {};
        do
        {
            status = GetDetectionStatus();
        } while (status.Calibrate == 1 && Nilai::GetTime() <= maxTime);

        // Make sure that the calibration is over.
        if (status.Calibrate == 1)
        {
            // Bit is still 1, calibration is not over yet.
            TS_ERROR("Calibration timed out after %i ms!", Nilai::GetTime() - startTime);
            return false;
        }
    }

    TS_DEBUG("Calibration complete! Took %ims.", Nilai::GetTime() - startTime);
    return true;
}

void At24Qt2120::Reset(bool waitForReset) noexcept
{
    TS_DEBUG("Resetting the sensor...");

    const uint8_t resetByte = 0xFF;
    TransmitFrameToRegister(
      s_i2cAddress, static_cast<uint8_t>(Registers::Reset), &resetByte, sizeof(resetByte));

    if (waitForReset)
    {
        // It can take up to 200ms after a reset for the sensor to respond to us again.
        static constexpr uint32_t RESET_TIME = 200;
        Nilai::Delay(RESET_TIME);

        if (CheckIfDevOnBus(s_i2cAddress))
        {
            TS_DEBUG("Reset complete!");
        }
        else
        {
            TS_ERROR("No response from sensor after reset!");
        }
    }
}

void At24Qt2120::SetSamplingInterval(size_t time) noexcept
{
    uint8_t regVal = SamplingTimeToRegVal(time);

    TS_DEBUG("Set LPM: %ims (%#02x)", time, regVal);

    TransmitFrameToRegister(
      s_i2cAddress, static_cast<uint8_t>(Registers::LowPowerMode), &regVal, sizeof(regVal));

    TS_VERIFY_VALUE(time, GetSamplingInterval());
}

size_t At24Qt2120::GetSamplingInterval() noexcept
{
    static constexpr uint8_t REG_SIZE = 1;

    I2C::Frame f = ReceiveFrameFromRegister(
      s_i2cAddress, static_cast<uint8_t>(Registers::LowPowerMode), REG_SIZE);

    return RegValToSamplingTime(f.data.front());
}

void At24Qt2120::SetTowardTouchDrift(uint8_t v) noexcept
{
    TS_DEBUG("Set TTD: %#02x", v);

    TransmitFrameToRegister(
      s_i2cAddress, static_cast<uint8_t>(Registers::TowardTouchDrift), &v, sizeof(v));

    TS_VERIFY_VALUE(v, GetTowardTouchDrift());
}

uint8_t At24Qt2120::GetTowardTouchDrift() noexcept
{
    static constexpr uint8_t REG_SIZE = 1;

    I2C::Frame f = ReceiveFrameFromRegister(
      s_i2cAddress, static_cast<uint8_t>(Registers::TowardTouchDrift), REG_SIZE);

    return f.data.front();
}

void At24Qt2120::SetAwayFromTouchDrift(uint8_t v) noexcept
{
    TS_DEBUG("Set AFD: %#02x", v);

    TransmitFrameToRegister(
      s_i2cAddress, static_cast<uint8_t>(Registers::AwayFromTouchDrift), &v, sizeof(v));

    TS_VERIFY_VALUE(v, GetAwayFromTouchDrift());
}

uint8_t At24Qt2120::GetAwayFromTouchDrift() noexcept
{
    static constexpr uint8_t REG_SIZE = 1;

    I2C::Frame f = ReceiveFrameFromRegister(
      s_i2cAddress, static_cast<uint8_t>(Registers::AwayFromTouchDrift), REG_SIZE);

    return f.data.front();
}

void At24Qt2120::SetDetectionIntegrator(uint8_t v) noexcept
{
    // Detection integrator value ranges from 1 to 32, with 0 being an alias for 1.
    v = std::max(v, static_cast<uint8_t>(32));
    TS_DEBUG("Set DI: %#02x", v);

    TransmitFrameToRegister(
      s_i2cAddress, static_cast<uint8_t>(Registers::DetectionIntegrator), &v, sizeof(uint8_t));

    TS_VERIFY_VALUE(v, GetDetectionIntegrator());
}

uint8_t At24Qt2120::GetDetectionIntegrator() noexcept
{
    static constexpr uint8_t REG_SIZE = 1;

    I2C::Frame f = ReceiveFrameFromRegister(
      s_i2cAddress, static_cast<uint8_t>(Registers::DetectionIntegrator), REG_SIZE);

    return f.data.front();
}

void At24Qt2120::SetTouchRecalibrationDelay(size_t delay) noexcept
{
    uint8_t regVal = DriftTimeToRegVal(delay);
    TS_DEBUG("Set TRD: %ims (%#02x)", delay, regVal);

    TransmitFrameToRegister(
      s_i2cAddress, static_cast<uint8_t>(Registers::TouchRecalDelay), &regVal, sizeof(regVal));

    TS_VERIFY_VALUE(delay, GetTouchRecalibrationDelay());
}

size_t At24Qt2120::GetTouchRecalibrationDelay() noexcept
{
    static constexpr uint8_t REG_SIZE = 1;

    I2C::Frame f = ReceiveFrameFromRegister(
      s_i2cAddress, static_cast<uint8_t>(Registers::TouchRecalDelay), REG_SIZE);

    return RegValToDriftTime(f.data.front());
}

void At24Qt2120::SetDriftHoldTime(size_t time) noexcept
{
    uint8_t regVal = DriftTimeToRegVal(time);
    TS_DEBUG("Set DHT: %ims (%#02x)", time, regVal);

    TransmitFrameToRegister(
      s_i2cAddress, static_cast<uint8_t>(Registers::DriftHoldTime), &regVal, sizeof(regVal));

    TS_VERIFY_VALUE(time, GetDriftHoldTime());
}

size_t At24Qt2120::GetDriftHoldTime() noexcept
{
    static constexpr uint8_t REG_SIZE = 1;

    I2C::Frame f = ReceiveFrameFromRegister(
      s_i2cAddress, static_cast<uint8_t>(Registers::DriftHoldTime), REG_SIZE);

    return RegValToDriftTime(f.data.front());
}

void At24Qt2120::SetSliderOptions(bool enable, bool wheel) noexcept
{
    TS_DEBUG(
      "Set Slider opt: EN: %i, Wheel: %i", static_cast<int>(enable), static_cast<int>(wheel));

    AT24QT2120::SliderOptions opt {enable, wheel};
    uint8_t                   regVal = static_cast<uint8_t>(opt);

    TransmitFrameToRegister(
      s_i2cAddress, static_cast<uint8_t>(Registers::SliderOption), &regVal, sizeof(regVal));

    TS_VERIFY_VALUE(opt, GetSliderOptions());
}

AT24QT2120::SliderOptions At24Qt2120::GetSliderOptions() noexcept
{
    static constexpr uint8_t REG_SIZE = 1;

    I2C::Frame f = ReceiveFrameFromRegister(
      s_i2cAddress, static_cast<uint8_t>(Registers::SliderOption), REG_SIZE);

    return AT24QT2120::SliderOptions {f.data.front()};
}

void At24Qt2120::SetChargeTime(uint8_t us) noexcept
{
    TS_DEBUG("Set Charge Time: %ius", us);

    TransmitFrameToRegister(
      s_i2cAddress, static_cast<uint8_t>(Registers::ChargeTime), &us, sizeof(us));

    TS_VERIFY_VALUE(us, GetChargeTime());
}

uint8_t At24Qt2120::GetChargeTime() noexcept
{
    static constexpr uint8_t REG_SIZE = 1;

    I2C::Frame f =
      ReceiveFrameFromRegister(s_i2cAddress, static_cast<uint8_t>(Registers::ChargeTime), REG_SIZE);

    return f.data.front();
}

void At24Qt2120::SetDetectionThreshold(AT24QT2120::Keys key, uint8_t threshold) noexcept
{
    // A value of 0 would result in a constant, unstoppable detection.
    threshold = std::min(threshold, static_cast<uint8_t>(1));

    TS_DEBUG("Set Key %i DTHR: %#02x", static_cast<int>(key), threshold);

    Registers r = RegisterFromKey(key, Registers::Key0DetectThreshold);

    TransmitFrameToRegister(s_i2cAddress, static_cast<uint8_t>(r), &threshold, sizeof(threshold));

    TS_VERIFY_VALUE(threshold, GetDetectionThreshold(key));
}

uint8_t At24Qt2120::GetDetectionThreshold(AT24QT2120::Keys key) noexcept
{
    static constexpr uint8_t REG_SIZE = 1;

    Registers r = RegisterFromKey(key, Registers::Key0DetectThreshold);

    I2C::Frame f = ReceiveFrameFromRegister(s_i2cAddress, static_cast<uint8_t>(r), REG_SIZE);

    return f.data.front();
}

void At24Qt2120::KeyIsGuard(AT24QT2120::Keys key, bool guard) noexcept
{
    KeyOptions options = GetKeyOptions(key);

    options.Guard = guard;
    SetKeyOptions(key, options);
}

void At24Qt2120::KeyInGroup(AT24QT2120::Keys key, AT24QT2120::Group group) noexcept
{
    KeyOptions options = GetKeyOptions(key);

    options.AKS = static_cast<uint8_t>(group);
    SetKeyOptions(key, options);
}

void At24Qt2120::SetKeyState(AT24QT2120::Keys key, bool state) noexcept
{
    KeyOptions options = GetKeyOptions(key);

    options.GPO = state;
    SetKeyOptions(key, options);
}

void At24Qt2120::KeyIsOutput(AT24QT2120::Keys key, bool output) noexcept
{
    KeyOptions options = GetKeyOptions(key);

    options.Enable = output;
    SetKeyOptions(key, options);
}

void At24Qt2120::SetKeyOptions(AT24QT2120::Keys key, const KeyOptions& options) noexcept
{
    uint8_t regVal = static_cast<uint8_t>(options);
    TS_DEBUG("Set key %i options (%#02x):\n\tGuard: %i\n\tGroup: %i\n\tState: %i\n\tOutput: %i",
             static_cast<int>(key),
             regVal,
             options.Guard,
             options.AKS,
             options.GPO,
             options.Enable);

    Registers r = RegisterFromKey(key, Registers::Key0Control);

    TransmitFrameToRegister(s_i2cAddress, static_cast<uint8_t>(r), &regVal, sizeof(regVal));

    TS_VERIFY_VALUE(options, GetKeyOptions(key));
}

AT24QT2120::KeyOptions At24Qt2120::GetKeyOptions(AT24QT2120::Keys key) noexcept
{
    static constexpr uint8_t REG_SIZE = 1;

    Registers  r = RegisterFromKey(key, Registers::Key0Control);
    I2C::Frame f = ReceiveFrameFromRegister(s_i2cAddress, static_cast<uint8_t>(r), REG_SIZE);

    return KeyOptions {f.data.front()};
}

void At24Qt2120::SetKeyPulse(AT24QT2120::Keys key, uint8_t pulse) noexcept
{
    AT24QT2120::PulseScale ps = GetKeyPulseScale(key);

    ps.Pulse = pulse;
    SetKeyPulseScale(key, ps);
}

void At24Qt2120::SetKeyScale(AT24QT2120::Keys key, uint8_t scale) noexcept
{
    AT24QT2120::PulseScale ps = GetKeyPulseScale(key);
    ps.Scale                  = scale;

    SetKeyPulseScale(key, ps);
}

void At24Qt2120::SetKeyPulseScale(AT24QT2120::Keys key, const AT24QT2120::PulseScale& ps) noexcept
{
    TS_DEBUG("Set key %i, pulse: %i, scale: %i", static_cast<int>(key), ps.Pulse, ps.Scale);

    uint8_t   regVal = static_cast<uint8_t>(ps);
    Registers r      = RegisterFromKey(key, Registers::Key0PulseScale);

    TransmitFrameToRegister(s_i2cAddress, static_cast<uint8_t>(r), &regVal, sizeof(regVal));

    TS_VERIFY_VALUE(ps, GetKeyPulseScale(key));
}

AT24QT2120::PulseScale At24Qt2120::GetKeyPulseScale(AT24QT2120::Keys key) noexcept
{
    static constexpr uint8_t REG_SIZE = 1;

    Registers r = RegisterFromKey(key, Registers::Key0PulseScale);

    I2C::Frame f = ReceiveFrameFromRegister(s_i2cAddress, static_cast<uint8_t>(r), REG_SIZE);

    return AT24QT2120::PulseScale {f.data.front()};
}

uint16_t At24Qt2120::GetKeySignal(AT24QT2120::Keys key) noexcept
{
    static constexpr uint8_t REG_SIZE = 2;

    Registers  r = RegisterFromKey(key, Registers::KeySignal0MSB);
    I2C::Frame f = ReceiveFrameFromRegister(s_i2cAddress, static_cast<uint8_t>(r), REG_SIZE);

    return (static_cast<uint16_t>(f.data[0]) << 8) | static_cast<uint16_t>(f.data[1]);
}

uint16_t At24Qt2120::GetKeyReference(AT24QT2120::Keys key) noexcept
{
    static constexpr uint8_t REG_SIZE = 2;

    Registers  r = RegisterFromKey(key, Registers::ReferenceData0MSB);
    I2C::Frame f = ReceiveFrameFromRegister(s_i2cAddress, static_cast<uint8_t>(r), REG_SIZE);

    return (static_cast<uint16_t>(f.data[0]) << 8) | static_cast<uint16_t>(f.data[1]);
}
#    endif
}    // namespace Nilai::Interfaces
#endif
