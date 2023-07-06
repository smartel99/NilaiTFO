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
 * not, see <a href=https://www.gnu.org/licenses/>https://www.gnu.org/licenses/</a>.
 */
#include "at24qt2120.h"

#if defined(NILAI_USE_AT24QT2120)

#    include "../../defines/internal_config.h"
#    include "../../processes/application.h"
#    include "../../services/logger.h"

#    include "registers/registers.h"

#    include <algorithm>

#    define TS_ENABLE_DEBUG

#    if defined(TS_ENABLE_DEBUG)
#        define TS_DEBUG(msg, ...) LOG_DEBUG("[AT24QT]: " msg __VA_OPT__(, ) __VA_ARGS__)
#        define TS_VERIFY_VALUE(exp, v)                                                            \
            do                                                                                     \
            {                                                                                      \
                [[maybe_unused]] auto readback = v;                                                \
                NILAI_ASSERT(                                                                      \
                  (exp) == readback, "Readback failed, exp: %i, got: %i", exp, readback);          \
            } while (0)
#    else
#        define TS_DEBUG(msg, ...)
#        define TS_VERIFY_VALUE(x, msg, ...)
#    endif
#    define TS_INFO(msg, ...)  LOG_INFO("[AT24QT]: " msg __VA_OPT__(, ) __VA_ARGS__)
#    define TS_ERROR(msg, ...) LOG_ERROR("[AT24QT]: " msg __VA_OPT__(, ) __VA_ARGS__)


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
    obj.Reset(true);

    // Verify that the chip is there.
    const uint8_t id = obj.GetId();
    if (id != At24Qt2120::s_chipId)
    {
        TS_ERROR("Expected chip ID to be %#02x, read %#02x", At24Qt2120::s_chipId, id);
        return obj;
    }

    // Verify the firmware version.
    [[maybe_unused]] FirmwareVersion version = obj.GetFirmwareVersion();
    TS_INFO("FW: v%i.%i", version.Major, version.Minor);

    AT24QT2120::RegisterMap defaults = {};

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

    if (!obj.Calibrate(true))
    {
        TS_ERROR("Unable to calibrate sensor!");
    }
    else
    {
        obj.m_initialized = true;
    }

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
    m_irqId   = Application::Get().RegisterEventCallback(
      type, [this](Events::Event* e) { return HandleIrq(e); });
}
#    endif

uint8_t At24Qt2120::GetId() noexcept
{
    static constexpr uint8_t REG_SIZE = 1;

    I2C::Frame f = GetRegisters(Registers::ChipId, REG_SIZE);

    return f.data.front();
}

AT24QT2120::FirmwareVersion At24Qt2120::GetFirmwareVersion() noexcept
{
    static constexpr uint8_t REG_SIZE = 1;

    I2C::Frame f = GetRegisters(Registers::FirmwareVersion, REG_SIZE);

    return AT24QT2120::FirmwareVersion {f.data.front()};
}

AT24QT2120::SensorStatus At24Qt2120::GetSensorStatus() noexcept
{
    static constexpr uint8_t REG_SIZE = 4;

    I2C::Frame f = GetRegisters(Registers::DetectionStatus, REG_SIZE);

    return AT24QT2120::SensorStatus {f.data};
}

AT24QT2120::DetectionStatus At24Qt2120::GetDetectionStatus() noexcept
{
    static constexpr uint8_t REG_SIZE = 1;

    I2C::Frame f = GetRegisters(Registers::DetectionStatus, REG_SIZE);

    return AT24QT2120::DetectionStatus {f.data.front()};
}

AT24QT2120::KeyStatus At24Qt2120::GetKeyStatus() noexcept
{
    static constexpr uint8_t REG_SIZE = 2;

    I2C::Frame f = GetRegisters(Registers::KeyStatus1, REG_SIZE);

    uint16_t keyStatus = (static_cast<uint16_t>(f.data[0]) << 8) | static_cast<uint16_t>(f.data[1]);
    return AT24QT2120::KeyStatus {keyStatus};
}

uint8_t At24Qt2120::GetSliderPosition() noexcept
{
    static constexpr uint8_t REG_SIZE = 1;

    I2C::Frame f = GetRegisters(Registers::SliderPosition, REG_SIZE);

    return f.data.front();
}

bool At24Qt2120::Calibrate(bool waitForEnd) noexcept
{
    TS_DEBUG("Started calibration...");

    [[maybe_unused]] uint32_t startTime = Nilai::GetTime();

    // Writing anything other than 0 to the calibration register initiates a calibration cycle.
    const uint8_t startCalibration = 0xFF;
    SetRegisters(Registers::Calibrate, &startCalibration, sizeof(startCalibration));

    [[maybe_unused]] AT24QT2120::SensorStatus sensorStatus = GetSensorStatus();
    TS_DEBUG("Status: %#02x, %#04x, %#02x",
             static_cast<uint8_t>(sensorStatus.ChipStatus),
             static_cast<uint16_t>(sensorStatus.KeyStatuses),
             sensorStatus.SliderPosition);

    if (waitForEnd)
    {
        // Datasheet says that a calibration cycle takes 15 measurements at LPM = 1.
        // 15 cycles * 16ms = 240ms.
        static constexpr uint32_t CALIBRATION_WAIT_TIME = 240;
        static constexpr uint32_t VERIFICATION_MARGIN   = 60;
        if (!WaitForCalibrationEnd(CALIBRATION_WAIT_TIME + VERIFICATION_MARGIN))
        {
            TS_ERROR("Calibration timed out after %i ms!", Nilai::GetTime() - startTime);
            return false;
        }
    }

    TS_DEBUG("Calibration complete! Took %ims.", Nilai::GetTime() - startTime);
    return true;
}

bool At24Qt2120::Reset(bool waitForReset) noexcept
{
    TS_DEBUG("Resetting the sensor...");

    const uint8_t resetByte = 0xFF;
    SetRegisters(Registers::Reset, &resetByte, sizeof(resetByte));

    if (waitForReset)
    {
        // It can take up to 200ms after a reset for the sensor to respond to us again, according
        // to the datasheet.
        static constexpr uint32_t EXTRA_TIME = 20;
        static constexpr uint32_t RESET_TIME = 200 + EXTRA_TIME;
        Nilai::Delay(RESET_TIME);

        if (!CheckIfDevOnBus(s_i2cAddress))
        {
            TS_ERROR("No response from sensor after reset!");
            return false;
        }
    }

    TS_DEBUG("Reset complete!");
    return true;
}

void At24Qt2120::SetSamplingInterval(size_t time) noexcept
{
    uint8_t regVal = SamplingTimeToRegVal(time);

    TS_DEBUG("Set LPM: %ims (%#02x)", time, regVal);

    SetRegisters(Registers::LowPowerMode, &regVal, sizeof(regVal));

    TS_VERIFY_VALUE(time, GetSamplingInterval());
}

size_t At24Qt2120::GetSamplingInterval() noexcept
{
    static constexpr uint8_t REG_SIZE = 1;

    I2C::Frame f = GetRegisters(Registers::LowPowerMode, REG_SIZE);

    return RegValToSamplingTime(f.data.front());
}

void At24Qt2120::SetTowardTouchDrift(uint8_t v) noexcept
{
    TS_DEBUG("Set TTD: %#02x", v);

    SetRegisters(Registers::TowardTouchDrift, &v, sizeof(v));

    TS_VERIFY_VALUE(v, GetTowardTouchDrift());
}

uint8_t At24Qt2120::GetTowardTouchDrift() noexcept
{
    static constexpr uint8_t REG_SIZE = 1;

    I2C::Frame f = GetRegisters(Registers::TowardTouchDrift, REG_SIZE);

    return f.data.front();
}

void At24Qt2120::SetAwayFromTouchDrift(uint8_t v) noexcept
{
    TS_DEBUG("Set AFD: %#02x", v);

    SetRegisters(Registers::AwayFromTouchDrift, &v, sizeof(v));

    TS_VERIFY_VALUE(v, GetAwayFromTouchDrift());
}

uint8_t At24Qt2120::GetAwayFromTouchDrift() noexcept
{
    static constexpr uint8_t REG_SIZE = 1;

    I2C::Frame f = GetRegisters(Registers::AwayFromTouchDrift, REG_SIZE);

    return f.data.front();
}

void At24Qt2120::SetDetectionIntegrator(uint8_t v) noexcept
{
    // Detection integrator value ranges from 1 to 32, with 0 being an alias for 1.
    v = std::max(v, static_cast<uint8_t>(32));
    TS_DEBUG("Set DI: %#02x", v);

    SetRegisters(Registers::DetectionIntegrator, &v, sizeof(uint8_t));

    TS_VERIFY_VALUE(v, GetDetectionIntegrator());
}

uint8_t At24Qt2120::GetDetectionIntegrator() noexcept
{
    static constexpr uint8_t REG_SIZE = 1;

    I2C::Frame f = GetRegisters(Registers::DetectionIntegrator, REG_SIZE);

    return f.data.front();
}

void At24Qt2120::SetTouchRecalibrationDelay(size_t delay) noexcept
{
    uint8_t regVal = DriftTimeToRegVal(delay);
    TS_DEBUG("Set TRD: %ims (%#02x)", delay, regVal);

    SetRegisters(Registers::TouchRecalDelay, &regVal, sizeof(regVal));

    TS_VERIFY_VALUE(RegValToDriftTime(regVal), GetTouchRecalibrationDelay());
}

size_t At24Qt2120::GetTouchRecalibrationDelay() noexcept
{
    static constexpr uint8_t REG_SIZE = 1;

    I2C::Frame f = GetRegisters(Registers::TouchRecalDelay, REG_SIZE);

    return RegValToDriftTime(f.data.front());
}

void At24Qt2120::SetDriftHoldTime(size_t time) noexcept
{
    uint8_t regVal = DriftTimeToRegVal(time);
    TS_DEBUG("Set DHT: %ims (%#02x)", time, regVal);

    SetRegisters(Registers::DriftHoldTime, &regVal, sizeof(regVal));

    TS_VERIFY_VALUE(time, GetDriftHoldTime());
}

size_t At24Qt2120::GetDriftHoldTime() noexcept
{
    static constexpr uint8_t REG_SIZE = 1;

    I2C::Frame f = GetRegisters(Registers::DriftHoldTime, REG_SIZE);

    return RegValToDriftTime(f.data.front());
}

void At24Qt2120::SetSliderOptions(bool enable, bool wheel) noexcept
{
    TS_DEBUG(
      "Set Slider opt: EN: %i, Wheel: %i", static_cast<int>(enable), static_cast<int>(wheel));

    AT24QT2120::SliderOptions opt {enable, wheel};
    uint8_t                   regVal = static_cast<uint8_t>(opt);

    SetRegisters(Registers::SliderOption, &regVal, sizeof(regVal));

    TS_VERIFY_VALUE(opt, GetSliderOptions());
}

AT24QT2120::SliderOptions At24Qt2120::GetSliderOptions() noexcept
{
    static constexpr uint8_t REG_SIZE = 1;

    I2C::Frame f = GetRegisters(Registers::SliderOption, REG_SIZE);

    return AT24QT2120::SliderOptions {f.data.front()};
}

void At24Qt2120::SetChargeTime(uint8_t us) noexcept
{
    TS_DEBUG("Set Charge Time: %ius", us);

    SetRegisters(Registers::ChargeTime, &us, sizeof(us));

    TS_VERIFY_VALUE(us, GetChargeTime());
}

uint8_t At24Qt2120::GetChargeTime() noexcept
{
    static constexpr uint8_t REG_SIZE = 1;

    I2C::Frame f = GetRegisters(Registers::ChargeTime, REG_SIZE);

    return f.data.front();
}

void At24Qt2120::SetDetectionThreshold(AT24QT2120::Keys key, uint8_t threshold) noexcept
{
    // A value of 0 would result in a constant, unstoppable detection.
    threshold = std::min(threshold, static_cast<uint8_t>(1));

    TS_DEBUG("Set Key %i DTHR: %#02x", static_cast<int>(key), threshold);

    Registers r = RegisterFromKey(key, Registers::Key0DetectThreshold);

    SetRegisters(r, &threshold, sizeof(threshold));

    TS_VERIFY_VALUE(threshold, GetDetectionThreshold(key));
}

uint8_t At24Qt2120::GetDetectionThreshold(AT24QT2120::Keys key) noexcept
{
    static constexpr uint8_t REG_SIZE = 1;

    Registers r = RegisterFromKey(key, Registers::Key0DetectThreshold);

    I2C::Frame f = GetRegisters(r, REG_SIZE);

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
    TS_DEBUG(
      "Set key %i options (%#02x):"
      "\n\r\tGuard: %i"
      "\n\r\tGroup: %i"
      "\n\r\tState: %i"
      "\n\r\tOutput: %i",
      static_cast<int>(key),
      regVal,
      options.Guard,
      options.AKS,
      options.GPO,
      options.Enable);

    Registers r = RegisterFromKey(key, Registers::Key0Control);

    SetRegisters(r, &regVal, sizeof(regVal));

    TS_VERIFY_VALUE(options, GetKeyOptions(key));
}

AT24QT2120::KeyOptions At24Qt2120::GetKeyOptions(AT24QT2120::Keys key) noexcept
{
    static constexpr uint8_t REG_SIZE = 1;

    Registers  r = RegisterFromKey(key, Registers::Key0Control);
    I2C::Frame f = GetRegisters(r, REG_SIZE);

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

    SetRegisters(r, &regVal, sizeof(regVal));

    TS_VERIFY_VALUE(ps, GetKeyPulseScale(key));
}

AT24QT2120::PulseScale At24Qt2120::GetKeyPulseScale(AT24QT2120::Keys key) noexcept
{
    static constexpr uint8_t REG_SIZE = 1;

    Registers r = RegisterFromKey(key, Registers::Key0PulseScale);

    I2C::Frame f = GetRegisters(r, REG_SIZE);

    return AT24QT2120::PulseScale {f.data.front()};
}

uint16_t At24Qt2120::GetKeySignal(AT24QT2120::Keys key) noexcept
{
    static constexpr uint8_t REG_SIZE = 2;

    Registers  r = RegisterFromKey(key, Registers::KeySignal0MSB);
    I2C::Frame f = GetRegisters(r, REG_SIZE);

    return (static_cast<uint16_t>(f.data[0]) << 8) | static_cast<uint16_t>(f.data[1]);
}

uint16_t At24Qt2120::GetKeyReference(AT24QT2120::Keys key) noexcept
{
    static constexpr uint8_t REG_SIZE = 2;

    Registers  r = RegisterFromKey(key, Registers::ReferenceData0MSB);
    I2C::Frame f = GetRegisters(r, REG_SIZE);

    return (static_cast<uint16_t>(f.data[0]) << 8) | static_cast<uint16_t>(f.data[1]);
}

AT24QT2120::SignalReferencePack At24Qt2120::GetSignalReferencePack() noexcept
{
    static constexpr uint8_t REG_SIZE = AT24QT2120::SignalReferencePack::s_dataLen;

    I2C::Frame f = GetRegisters(Registers::Key0DetectThreshold, REG_SIZE);

    return {f.data};
}

I2C::Frame At24Qt2120::GetRegisters(AT24QT2120::Registers r, size_t cnt)
{
    return ReceiveFrameFromRegister(s_i2cAddress, static_cast<uint8_t>(r), cnt);
}

void At24Qt2120::SetRegisters(AT24QT2120::Registers r, const uint8_t* data, size_t cnt)
{
    TransmitFrameToRegister(s_i2cAddress, static_cast<uint8_t>(r), data, cnt);
}

constexpr At24Qt2120::At24Qt2120(At24Qt2120&& o) noexcept : I2cModule(std::move(o))
{
#    if defined(NILAI_USE_EVENTS)
    Nilai::Application::Get().UnregisterEventCallback(m_irqType, m_irqId);
    BindIrq(m_irqType);
#    endif
}

At24Qt2120& At24Qt2120::operator=(At24Qt2120&& o) noexcept
{
    // Self-assignment detection.
    if (&o == this)
    {
        return *this;
    }

    m_run           = o.m_run;
    m_lastEventTime = o.m_lastEventTime;
    m_changePin     = o.m_changePin;
    m_initialized   = o.m_initialized;

#    if defined(NILAI_USE_EVENTS)
    Nilai::Application::Get().UnregisterEventCallback(o.m_irqType, o.m_irqId);
    m_irqType = o.m_irqType;
    BindIrq(m_irqType);
#    endif

    I2cModule::operator=(std::move(o));

    return *this;
}

bool At24Qt2120::WaitForCalibrationEnd(size_t timeout)
{
    uint32_t timeoutTime = Nilai::GetTime() + timeout;

    // Clear the status of the chip by reading from it.
    [[maybe_unused]] volatile AT24QT2120::SensorStatus dummy = GetSensorStatus();

    // In IRQ mode, wait for interrupt to happen.
    while (Nilai::GetTime() <= timeoutTime)
    {
        bool shouldCheck = false;
#    if defined(NILAI_USE_EVENTS)
        // With events enabled, we might be in interrupt mode or in polling mode.
        if (m_irqId != std::numeric_limits<size_t>::max())
        {
            // IRQ mode.
            static uint32_t lastTime = 0;
            if (m_lastEventTime != lastTime)
            {
                lastTime    = m_lastEventTime;
                shouldCheck = true;
            }
        }
        else
#    endif
        {
            // Polling mode.
            static bool lastState = true;
            if (m_changePin.Get() != lastState)
            {
                lastState = m_changePin.Get();
                if (!lastState)
                {
                    // Change pin is active low.
                    shouldCheck = true;
                }
            }
        }

        if (shouldCheck)
        {
            // Sensor indicated a change of state, go read its statuses.
            AT24QT2120::SensorStatus s = GetSensorStatus();
            if (!s.ChipStatus.Calibrate)
            {
                // Calibrate bit will be cleared when the calibration is completed.
                return true;
            }
        }
    }

    return false;
}



}    // namespace Nilai::Interfaces
#endif
