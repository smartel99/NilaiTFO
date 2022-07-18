/**
 * @file    at24qt2120.h
 * @author  Samuel Martel
 * @date    2022-06-06
 * @brief   Header for the AT24QT2120 capacitive touch sensing circuit.
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


#ifndef GUARD_AT24QT2120_H
#define GUARD_AT24QT2120_H

#if defined(NILAI_USE_AT24QT2120)

#    include "../../defines/pin.h"
#    include "../../drivers/i2c_module.h"

#    if defined(NILAI_USE_EVENTS)
#        include "../../defines/events/events.h"
#    endif

#    include "default_values.h"
#    include "registers/registers.h"

namespace Nilai::Interfaces
{
/**
 * @class At24Qt2120
 *
 * @note
 * <a href=https://ww1.microchip.com/downloads/en/DeviceDoc/doc9634.pdf>Datasheet</a>
 *
 * @brief Class interfacing with the AT24QT2120 capacitive touch sensing IC.
 *
 * <h2>Theory of operation:</h2>
 *
 * <dl>
 * <dt><b>Adjacent Key Suppression (AKS):</b></dt>
 *      <dd>This feature allows the use of tightly spaced keys with reduced interference between
 *      each one of them.
 *  </dd><dt></dt><dd>
 *      Up to three AKS groups can be used simultaneously, and there can only be one key in a
 *      group reported as being touched at any one time. Once a key in an AKS goes into "detect
 *      mode", no other key in that group can be detected until the first key is released.
 *  </dd><dt></dt><dd>
 *      <b>Note:</b> To use a key as a guard channel, its AKS group should be set to be the same
 *      as that of the keys it is to protect.
 *      </dd>
 * </dl>
 *
 * <dl>
 *  <dt><b>The CHANGE pin:</b></dt>
 *      <dd>The CHANGE pin is pulled low by the chip whenever there is a change of state in the
 *      Detection status and/or Key Status bytes. It is cleared when the status bytes are read.
 *  </dd><dt></dt><dd>
 *      After a power-up reset, the CHANGE line is pulled <b>LOW</b> for 85ms.
 *  </dd><dt></dt><dd>
 *      This pin is also pulled <b>LOW</b> for another 16ms before any bursting on the touch pins
 *      occurs. If the state of any output needs to be changed, the new state needs to be written
 *      during this period.
 *      </dd>
 * </dl>
 *
 * <dl>
 *  <dt><b>Types of Reset:</b></dt>
 *  <dd><dl>
 *      <dt>External Reset</dt>
 *          <dd>An (optional) external reset line can be used if desired. This signal is active
 *          <b>LOW</b>, and needs to be active for a minimum of 2us.
 *          </dd>
 *      <dt>Soft Reset</dt>
 *          <dd>
 *          The Reset register can be used to trigger a software. The reception of this command
 *          starts the internal watchdog timer of the IC, which expires after 125ms. Upon
 *          expiring, the IC executes a full reset.
 *          </dd><dt></dt><dd>
 *          The IC will not respond to any command received through I2C for approximately 200ms
 *          after the reset command has been received.
 *          </dd><dt></dt><dd>
 *          <b>Note:</b> The IC can receive a reset command even while it is in Power Down mode.
 *          </dd>
 *  </dl></dd>
 * </dl>
 *
 * <dl>
 *  <dt><b>Calibration:</b></dt>
 *      <dd>Writing any non zero value into the calibration register forces the IC to
 *      re-calibrate itself. This can be useful to clear out a key that is stuck, i.e. a key that
 *      have been detected as pressed for an unrealistic amount of time.
 *      </dd><dt></dt>
 *      <dd>The calibration command executes 15 burst cycles at a sampling rate mode of 1, as
 *      well as maintaining the <b>CALIBRATE</b> bit of the Detection Status to 1 for as long as
 *      the calibration is on-going.
 *      </dd><dt></dt>
 *      <dd><b>Note: </b>Calibration should be done whenever the Key Control bit 0 (EN) is changed.
 *      This changes the use of the key form a standard touch key to an output pin and vice-versa.
 *      </dd>
 * </dl>
 *
 * <dl>
 *  <dt><b>Guard Channel:</b></dt>
 *      <dd>A guard channel can be used to help prevent false detections on the key inputs. The
 *      channel which acts as the guard can be set through the Key Control bit 4 (GUARD).
 *      </dd><dt></dt>
 *      <dd>Due to the larger footprints of the guard channel, it becomes more susceptible to
 *      noise, therefore it should have a higher over-sampling rate than the other keys.
 *      </dd>
 * </dl>
 *
 * <dl>
 *  <dt><b>Signal Processing:</b></dt>
 *      <dd><dl>
 *          <dt><b>Detect Threshold:</b></dt>
 *          <dd>The device detects a touch when the signal has crossed a threshold level and
 *          remained there for a specified number of counts. This behavior can be altered on a
 *          key-by-key basis using the key Detect Threshold commands.
 *          </dd><dt></dt>
 *          <dd>This detect threshold is based on the reference value of the particular key. The
 *          delta of the key is obtained by subtracting the reference value from the signal
 *          value, which rises when a touch is present.
 *          </dd><dt></dt>
 *          <dd>The reference point of a key automatically drifts towards and away from the
 *          signal value at a configurable rate, which defaults to <i>160ms * 20 = 3.2
 *          seconds</i> towards the Touch Drift register, and to <i>160ms * 5 = 0.8 seconds</i>
 *          away from the Touch Drift register.
 *          </dd>
 *      </dl></dd>
 *      <dd><dl>
 *          <dt><b>Detection Integrator: </b></dt>
 *          <dd>The IC features a fast detection integrator counter/filter (DI filter), which is
 *          utilized to remove electrical noise at the small expense of a slower response time.
 *          </dd><dt></dt>
 *          <dd>For a touch to be registered as such, it needs to be detected for a programmable
 *          number of consecutive samples. The minimum number of consecutive samples is 1, with
 *          the maximum number being 32. Any value above 32 will result in a key never being
 *          registered as being touched, effectively disabling it.
 *          </dd><dt></dt>
 *          <dd>This filter is also present to detect the release of a key, using the same
 *          threshold value.
 *          </dd>
 *      </dl></dd>
 *      <dd><dl>
 *          <dt><b>Touch Recalibration Delay: </b></dt>
 *          <dd>An object placed in front of the key pad might result in erroneous detections,
 *          preventing the user from properly interacting with the device. To prevent this, the
 *          IC uses a timer which periodically monitors detections. If a detection exceeds the
 *          timer setting, the IC automatically performs a key recalibration. This is known as
 *          the Touch Recalibration Delay.
 *          </dd><dt></dt>
 *          <dd>This delay can be changed to any value between 160ms and 40800ms, in steps of
 *          160ms. The touch recalibration delay can also be disabled by using a delay of 0ms.
 *          </dd><dt></dt>
 *          <dd>This feature is global to the IC, covering every key.
 *          </dd>
 *      </dl></dd>
 *      <dd><dl>
 *          <dt><b>Away from Touch Recalibration:</b></dt>
 *          <dd>If a signal coming from a key jumps in the negative direction (with respect to
 *          its reference) by more than the Away from Touch Recalibration setting (25% of the
 *          detection threshold), then a recalibration of that key takes place.
 *          </dd><dt></dt>
 *          <dd><b>Note: </b>The minimum Away from Touch Recalibration threshold is hard
 *          limited to 4 counts.
 *          </dt>
 *      </dl></dd>
 *      <dd><dl>
 *          <dt><b>Drift Hold Time:</b></dt>
 *          <dd>Drift Hold Time (DHT) is used to restrict drift on all keys while one or more
 *          keys are active. DHT restricts the drifting on all keys until approximately four
 *          seconds after all touches have been removed.
 *          </dd><dt></dt>
 *          <dd>This feature is particularly useful in cases of high-density keypads, where
 *          touching a key or hovering a finger over the keypad would cause untouched keys to
 *          drift, and therefore create a sensitivity shift, and ultimately inhibit touch detection.
 *          The value of the DHT can be configured, with a default of ~4 seconds.
 *      </dl></dd>
 *      <dd><dl>
 *          <dt><b>Hysteresis:</b></dt>
 *          <dd>Hysteresis is fixed by default at 12.5% of the Detect Threshold. When a key
 *          enters a detect state once the DI count has been reached, the Detect Threshold (DTHR)
 *          value is changed by a small amount (12.5% of DTHR) in the direction away from the
 *          touch. This is done to effect hysteresis and thus makes it less likely that a key
 *          will dither in and out of detection. DTHR is restored once the key drops out of
 *          detection.
 *          </dd><dt></dt>
 *          <dd><b>Note: </b>The minimum value for hysteresis is 2 counts.
 *          </dd>
 *      </dl></dd>
 * </dl>
 */
class At24Qt2120 : public Nilai::Drivers::I2cModule
{
    using RunFunction = void (*)(At24Qt2120*);

public:
    class Builder
    {
        using Self = Builder;

    public:
        class KeyBuilder
        {
        public:
            static constexpr KeyBuilder Create(Builder& parent, AT24QT2120::Keys key)
            {
                return {parent, key};
            }

            constexpr Builder& Complete() { return m_parent; }

            constexpr KeyBuilder& SetDetectionThreshold(uint8_t threshold)
            {
                m_parent.m_values.Keys[static_cast<size_t>(m_key)].DetectThreshold = threshold;

                return *this;
            }

            constexpr KeyBuilder& IsGuard(bool isGuard)
            {
                m_parent.m_values.Keys[static_cast<size_t>(m_key)].Configuration.Guard = isGuard;

                return *this;
            }

            constexpr KeyBuilder& InGroup(AT24QT2120::Group group)
            {
                m_parent.m_values.Keys[static_cast<size_t>(m_key)].Configuration.AKS =
                  static_cast<uint8_t>(group);

                return *this;
            }

            constexpr KeyBuilder& OutputHigh(bool state)
            {
                m_parent.m_values.Keys[static_cast<size_t>(m_key)].Configuration.GPO = state;

                return *this;
            }

            constexpr KeyBuilder& IsOutput(bool isOutput)
            {
                m_parent.m_values.Keys[static_cast<size_t>(m_key)].Configuration.Enable = isOutput;

                return *this;
            }

            constexpr KeyBuilder& SetPulse(uint8_t pulse)
            {
                m_parent.m_values.Keys[static_cast<size_t>(m_key)].PulseAndScale.Pulse = pulse;

                return *this;
            }
            constexpr KeyBuilder& SetScale(uint8_t scale)
            {
                m_parent.m_values.Keys[static_cast<size_t>(m_key)].PulseAndScale.Scale = scale;

                return *this;
            }

        private:
            constexpr KeyBuilder(Builder& parent, AT24QT2120::Keys key)
            : m_parent(parent), m_key(key)
            {
            }

            Builder&         m_parent;
            AT24QT2120::Keys m_key;
        };

    public:
        constexpr explicit Builder(I2C_HandleTypeDef* i2c) noexcept : m_i2c(i2c) {}
        constexpr ~Builder() noexcept = default;

        constexpr Self& UseInPollingMode(Pin pin)
        {
#    if defined(NILAI_USE_EVENTS)
            // If the chip was in interrupt mode, leave it.
            m_irqType     = Events::EventTypes::Count;
            m_isInIrqMode = false;
#    endif
            m_pin = pin;

            return *this;
        }

#    if defined(NILAI_USE_EVENTS)
        constexpr Self& UseInInterruptMode(Events::EventTypes type)
        {
            NILAI_ASSERT(Events::IsInCategory(type, Events::EventCategories::External),
                         "Event must be external!");

            m_irqType     = type;
            m_isInIrqMode = true;

            return *this;
        }
#    endif

        constexpr Self& SetSamplingInterval(size_t time)
        {
            m_values.SamplingInterval = time;

            return *this;
        }

        constexpr Self& SetTowardTouchDrift(uint8_t v)
        {
            m_values.TowardTouchDrift = v;

            return *this;
        }

        constexpr Self& SetAwayFromTouchDrift(uint8_t v)
        {
            m_values.AwayFromTouchDrift = v;

            return *this;
        }

        constexpr Self& SetDetectionIntegrator(uint8_t v)
        {
            // Detection integrator value ranges from 1 to 32.
            m_values.DetectionIntegrator = std::max(v, static_cast<uint8_t>(32));

            return *this;
        }
        constexpr Self& SetTouchRecalibrationDelay(size_t delay)
        {
            m_values.TouchRecalibrationDelay = At24Qt2120::DriftTimeToRegVal(delay);

            return *this;
        }
        constexpr Self& SetDriftHoldTime(size_t time)
        {
            m_values.DriftHoldTime = At24Qt2120::DriftTimeToRegVal(time);

            return *this;
        }
        constexpr Self& SetSliderOptions(bool enable, bool wheel)
        {
            m_values.SliderOption = {enable, wheel};

            return *this;
        }
        constexpr Self& SetChargeTime(uint8_t us)
        {
            m_values.ChargeTime = us;

            return *this;
        }

        constexpr KeyBuilder SetKeyOptions(AT24QT2120::Keys key)
        {
            return KeyBuilder::Create(*this, key);
        }

        [[nodiscard]] At24Qt2120 Build() const;

    private:
        I2C_HandleTypeDef* m_i2c = nullptr;

        AT24QT2120::RegisterMap m_values = {};

#    if defined(NILAI_USE_EVENTS)
        bool               m_isInIrqMode = false;
        Events::EventTypes m_irqType     = Events::EventTypes::Count;
#    endif
        Pin m_pin = {};
    };

    static constexpr Builder Create(I2C_HandleTypeDef* i2c) { return Builder(i2c); }

    /**
     * @brief Get the last time at which the touch sensor detected a change in its state.
     * @returns The timestamp of the last event, expressed in milliseconds since startup.
     */
    [[nodiscard]] uint32_t GetLastEventTime() const;

    bool DoPost() override;
    void Run() override;

    /**
     * Gets the chip's ID.
     * @return Should always be 0x3E.
     */
    [[nodiscard]] uint8_t GetId() noexcept;

    /**
     * Gets the firmware version of the chip.
     * @return The firmware version.
     */
    [[nodiscard]] AT24QT2120::FirmwareVersion GetFirmwareVersion() noexcept;

    /**
     * Gets the detection status of the chip.
     * @return The detection status.
     */
    [[nodiscard]] AT24QT2120::DetectionStatus GetDetectionStatus() noexcept;

    /**
     * Gets the status of each keys.
     * @return The status of the keys.
     */
    [[nodiscard]] AT24QT2120::KeyStatus GetKeyStatus() noexcept;

    /**
     * Gets the position of the slider/wheel.
     * @return The position of the slider/wheel.
     *
     * @note This value is only valid when the SDET bit in the Detection Status register is set.
     */
    [[nodiscard]] uint8_t GetSliderPosition() noexcept;

    /**
     * @brief Triggers the device's calibration cycle.
     * @param waitForEnd If true, the function will wait for the calibration to end. Otherwise,
     * it will only start the calibration and instantly return.
     * @returns True if the calibration was successful, or that we did not wait for its end.
     * @returns False if the calibration did not finish in time.
     *
     * @note The calibration sequence can take up to 250ms.
     */
    bool Calibrate(bool waitForEnd = false) noexcept;

    /**
     * @brief Triggers a software reset on the chip.
     * @param waitForReset If true, wait for the sensor to be in operational state after
     * triggering the reset.
     *
     * @note If <b>waitForReset</b> is set, this method will block for up to 210ms.
     */
    void Reset(bool waitForReset = false) noexcept;

    /**
     * @brief Sets the interval between key measurements.
     *
     * Longer intervals between measurements yield a lower power consumption, at the expense of a
     * slower response to touch.
     *
     * A value of 0 puts the device in power-down mode, where no measurements are done.
     *
     * Valid values range from 16ms to 4.08 seconds, and is rounded down towards the nearest
     * multiple of 16ms.
     *
     * Defaults to 1 (16ms between key acquisitions).
     *
     * @param time The interval between each samples, in milliseconds.
     */
    void                 SetSamplingInterval(size_t time) noexcept;
    [[nodiscard]] size_t GetSamplingInterval() noexcept;

    /**
     * @brief Adjusts the Toward Touch Drift characteristics.
     *
     * Defaults to 20 (3.2s / reference level)
     *
     * Refer to the datasheet for more information.
     *
     * @param v Adjustment rates, in increments of 0.16s.
     */
    void                  SetTowardTouchDrift(uint8_t v) noexcept;
    [[nodiscard]] uint8_t GetTowardTouchDrift() noexcept;

    /**
     * @brief Adjusts the Away from Touch Drift characteristics.
     *
     * Defaults to 5 (0.8s / reference level).
     *
     * Refer to the datasheet for more information.
     *
     * @param v Adjustment rates, in increments of 0.16s.
     */
    void                  SetAwayFromTouchDrift(uint8_t v) noexcept;
    [[nodiscard]] uint8_t GetAwayFromTouchDrift() noexcept;

    /**
     * @brief Sets the number of consecutive measurements that must be confirmed as having passed
     * the key threshold before that key is registered as being in detection.
     *
     * Defaults to 4.
     *
     * @param v A value between 1 and 32.
     *
     * @note A value of 0 is the same as a value of 1.
     */
    void                  SetDetectionIntegrator(uint8_t v) noexcept;
    [[nodiscard]] uint8_t GetDetectionIntegrator() noexcept;

    /**
     * @brief Sets the amount of time a key can be held before a recalibration is initiated.
     *
     * This is to be able to ignore objects making contact with the detection surface.
     *
     * A value of 0 results in a recalibration never being requested.
     *
     * Values can range from 0.16s to 40.8s, in increments of 0.16 seconds.
     *
     * Defaults to 40.5 seconds.
     *
     * @param delay The delay in milliseconds. Rounded down to the nearest multiple of 16.
     */
    void                 SetTouchRecalibrationDelay(size_t delay) noexcept;
    [[nodiscard]] size_t GetTouchRecalibrationDelay() noexcept;

    /**
     * @brief Sets the length of time the drifting is halted after a key detection.
     *
     * When this value is 0, drifting is never suspended, even during a valid touch of another key.
     *
     * Values can range from 0.16 seconds to 40.8 seconds, in increments of 0.16 seconds.
     *
     * Defaults to 4 seconds.
     *
     * @param time The drift hold time, in milliseconds. Rounded down to the nearest multiple of 16.
     */
    void                 SetDriftHoldTime(size_t time) noexcept;
    [[nodiscard]] size_t GetDriftHoldTime() noexcept;

    /**
     * Sets the slider options.
     *
     * Defaults to disabled.
     *
     * @param enable Enables the slider/wheel.
     * @param wheel Marks the slider as being a wheel.
     */
    void SetSliderOptions(AT24QT2120::SliderOptions opts) noexcept
    {
        SetSliderOptions(opts.Enable, opts.Wheel);
    }
    void                                    SetSliderOptions(bool enable, bool wheel) noexcept;
    [[nodiscard]] AT24QT2120::SliderOptions GetSliderOptions() noexcept;

    /**
     * Prolongs the charge-transfer period of the signal acquisition by X microseconds.
     *
     * @param us The number of microseconds to prolong the acquisition by.
     */
    void                  SetChargeTime(uint8_t us) noexcept;
    [[nodiscard]] uint8_t GetChargeTime() noexcept;

    /**
     * Sets the detection threshold for the specified key.
     *
     * Defaults to 10.
     *
     * @param key The key to set.
     * @param threshold The threshold of that key.
     *
     * @attention Do not use a setting of 0 as this causes a key to go into detection when its
     * signal is equal to its reference.
     */
    void                  SetDetectionThreshold(AT24QT2120::Keys key, uint8_t threshold) noexcept;
    [[nodiscard]] uint8_t GetDetectionThreshold(AT24QT2120::Keys key) noexcept;

    /**
     * Mark the key as being a guard channel or not. A guard channel doesn't trigger any
     * detection signals.
     *
     * Defaults to false (not a guard).
     *
     * @param key The key to operate on.
     * @param guard If true, mark the key as a guard.
     */
    void KeyIsGuard(AT24QT2120::Keys key, bool guard) noexcept;

    /**
     * @brief Sets a key's group.
     *
     * Group 0 disables the grouping feature.
     *
     * Only one key per group can be detected as touched simultaneously.
     *
     * Defaults to Group 0 for all keys.
     *
     * @param key The key to operate on.
     * @param group The group to attach the key to.
     */
    void KeyInGroup(AT24QT2120::Keys key, AT24QT2120::Group group) noexcept;

    /**
     * @brief Sets the state of an output channel.
     *
     * This does nothing if the channel is not an output.
     *
     * Defaults to 0 for all keys.
     *
     * @param key The key to operate on.
     * @param state The state to set the key in.
     */
    void SetKeyState(AT24QT2120::Keys key, bool state) noexcept;

    /**
     * @brief Sets the mode of a channel.
     *
     * This either sets it as a key/slider input, or as an output.
     *
     * Defaults to input for every channels.
     *
     * @param key The key to operate on.
     * @param output The mode of the channel. True makes it an output, false makes it an input.
     */
    void KeyIsOutput(AT24QT2120::Keys key, bool output) noexcept;

    /**
     * @brief Gets the current options for the desired key.
     *
     * @param key The key to operate on.
     * @return The current options for the key.
     */
    void SetKeyOptions(AT24QT2120::Keys key, const AT24QT2120::KeyOptions& options) noexcept;
    [[nodiscard]] AT24QT2120::KeyOptions GetKeyOptions(AT24QT2120::Keys key) noexcept;

    /**
     * Refer to the datasheet for the exact details of modifying the Pulse/Scale of keys.
     *
     * Defaults to 0 for every key.
     *
     * @param key The key to operate on.
     * @param pulse The pulse value.
     */
    void SetKeyPulse(AT24QT2120::Keys key, uint8_t pulse) noexcept;

    /**
     * Refer to the datasheet for the exact details of modifying the Pulse/Scale of keys.
     *
     * Defaults to 0 for every key.
     *
     * @param key The key to operate on.
     * @param pulse The pulse value.
     */
    void SetKeyScale(AT24QT2120::Keys key, uint8_t scale) noexcept;

    void SetKeyPulseScale(AT24QT2120::Keys key, const AT24QT2120::PulseScale& ps) noexcept;
    [[nodiscard]] AT24QT2120::PulseScale GetKeyPulseScale(AT24QT2120::Keys key) noexcept;

    /**
     * @brief Gets the signal strength read by a key.
     *
     * @param key The key to fetch
     * @return The signal read by that key.
     */
    [[nodiscard]] uint16_t GetKeySignal(AT24QT2120::Keys key) noexcept;

    /**
     * @brief Gets the reference level of a key.
     *
     * @param key The key to fetch.
     * @return The reference for that key.
     */
    [[nodiscard]] uint16_t GetKeyReference(AT24QT2120::Keys key) noexcept;

    /**
     * @brief Provided only as a mean to have un-initialized instances.
     * Using an un-itialized instance will result in UB.
     */
    constexpr At24Qt2120() = default;

private:
    constexpr At24Qt2120(I2C_HandleTypeDef* i2c) noexcept
    : I2cModule(i2c, "AT24QT I2C"), m_run(&PollingRun)
    {
    }

    static void DefaultRun([[maybe_unused]] At24Qt2120* self) {}
    static void PollingRun(At24Qt2120* self);

#    if defined(NILAI_USE_EVENTS)
    static void IrqRun(At24Qt2120* self);
    void        BindIrq(Events::EventTypes type);
    bool        HandleIrq(Events::Event* e);
#    endif

    static constexpr uint8_t SamplingTimeToRegVal(size_t time)
    {
        // Register value = number of 16ms.
        return time / 16;
    }

    static constexpr size_t RegValToSamplingTime(uint8_t regVal)
    {
        // Register value = number of 16ms.
        return regVal * 16;
    }

    static constexpr uint8_t DriftTimeToRegVal(size_t time)
    {
        // Register value = number of 160ms.
        return time / 160;
    }

    static constexpr size_t RegValToDriftTime(uint8_t regVal)
    {
        // Register value = number of 160ms.
        return regVal * 160;
    }

    static constexpr AT24QT2120::Registers RegisterFromKey(AT24QT2120::Keys      key,
                                                           AT24QT2120::Registers firstReg)
    {
        return static_cast<AT24QT2120::Registers>(
          (static_cast<size_t>(firstReg) + static_cast<size_t>(key)));
    }

private:
    friend class At24Qt2120Builder;
    //! Function called in Run. This depends on the mode of operation, polling or interrupt-based.
    RunFunction m_run = &DefaultRun;

    //! Time of the last event detected by the touch sensor.
    uint32_t m_lastEventTime = 0;


private:
    //! When in Polling mode, represents the pin to monitor.
    Pin m_changePin = {};

#    if defined(NILAI_USE_EVENTS)
    //! The ID of the event callback.
    size_t m_irqId = std::numeric_limits<size_t>::max();
    //! When in Interrupt mode, represents the event that triggers the IRQ.
    Events::EventTypes m_irqType = Events::EventTypes::Exti0;
#    endif

    static constexpr uint8_t s_i2cAddress = 0x1C;
    static constexpr uint8_t s_chipId     = 0x3E;
};
}    // namespace Nilai::Interfaces

#endif

#endif    // GUARD_AT24QT2120_H
