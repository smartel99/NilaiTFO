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
 * not, see <a href=https://www.gnu.org/licenses/>https://www.gnu.org/licenses/</a>.
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

#    include <limits>

namespace Nilai::Interfaces
{
/**
 * @addtogroup Nilai
 * @{
 */

/**
 * @addtogroup Interfaces
 * @{
 */

/**
 * @addtogroup AT24QT2120
 * @{
 */

/**
 * @class At24Qt2120
 *
 * @note
 * <a href=https://ww1.microchip.com/downloads/en/DeviceDoc/doc9634.pdf>Datasheet</a>
 *
 * @brief Class interfacing with the AT24QT2120 capacitive touch sensing IC.
 *
 *
 * <b>Utilization:</b>
 * Before using the At24Qt2120 instance, it must first be initialized through the @c Builder
 * described bellow.
 *
 * Once created, simply query the time of the last event using @ref GetLastEventTime and get the
 * state of the sensor with @ref GetSensorStatus.
 *
 * @example interfaces/at24qt2120.cpp
 */
class At24Qt2120 : public Nilai::Drivers::I2cModule
{
    /**
     * @brief Alias for a pointer to the run function.
     */
    using RunFunction = void (*)(At24Qt2120*);

public:
    /**
     * @class Builder
     * @brief Builder for the At24Qt2120 module.
     */
    class Builder
    {
        using Self = Builder;

    public:
        /**
         * @class KeyBuilder
         * @brief Builder for key options.
         */
        class KeyBuilder
        {
        public:
            /**
             * @brief Creates a key builder for the specified key.
             * @param parent A reference to the owner of that key.
             * @param key The key to build upon.
             * @return A new, default instance of KeyBuilder
             */
            static constexpr KeyBuilder Create(Builder& parent, AT24QT2120::Keys key)
            {
                return {parent, key};
            }

            /**
             * @brief Terminates the configuration of the key.
             * @return The parent builder.
             */
            constexpr Builder& Complete() { return m_parent; }

            /**
             * @brief Sets the detection threshold for the key.
             * @param threshold The value that corresponds to the detection level.
             * @return A reference to the key builder.
             */
            constexpr KeyBuilder& SetDetectionThreshold(uint8_t threshold)
            {
                m_parent.m_values.Keys[static_cast<size_t>(m_key)].DetectThreshold = threshold;

                return *this;
            }

            /**
             * @brief Toggles the guard channel for this key.
             * @param isGuard True if the key is a guard channel, false otherwise.
             * @return A reference to the key builder.
             */
            constexpr KeyBuilder& IsGuard(bool isGuard)
            {
                m_parent.m_values.Keys[static_cast<size_t>(m_key)].Configuration.Guard = isGuard;

                return *this;
            }

            /**
             * @brief Sets the group in which the key belongs in.
             * @param group The group of the key.
             * @return A reference to the key builder.
             */
            constexpr KeyBuilder& InGroup(AT24QT2120::Group group)
            {
                m_parent.m_values.Keys[static_cast<size_t>(m_key)].Configuration.AKS =
                  static_cast<uint8_t>(group);

                return *this;
            }

            /**
             * @brief Sets the output state of the key.
             * @note The key must be set as an output using @ref IsOutput for this function to do
             * anything.
             * @param state True to set the key to HIGH, false to set it to LOW.
             * @return A reference to the key builder.
             */
            constexpr KeyBuilder& OutputHigh(bool state)
            {
                m_parent.m_values.Keys[static_cast<size_t>(m_key)].Configuration.GPO = state;

                return *this;
            }

            /**
             * @brief Controls whether the key is an input or an output.
             * @note When the key is set as an output, reading its state is pointless.
             * @param isOutput True if the key should be an output, false if it should be an input.
             * @return A reference to the key builder.
             */
            constexpr KeyBuilder& IsOutput(bool isOutput)
            {
                m_parent.m_values.Keys[static_cast<size_t>(m_key)].Configuration.Enable = isOutput;

                return *this;
            }

            /**
             * @brief Sets the pulse level of the key.
             * @note Refer to the <a
             * href=https://ww1.microchip.com/downloads/en/DeviceDoc/doc9634.pdf>datasheet</a> to
             * understand what this does.
             * @param pulse The pulse value. Valid range is [0..127]
             * @return A reference to the key builder.
             */
            constexpr KeyBuilder& SetPulse(uint8_t pulse)
            {
                m_parent.m_values.Keys[static_cast<size_t>(m_key)].PulseAndScale.Pulse = pulse;

                return *this;
            }

            /**
             * @brief Sets the scale level of the key.
             * @note Refer to the <a
             * href=https://ww1.microchip.com/downloads/en/DeviceDoc/doc9634.pdf>datasheet</a> to
             * understand what this does.
             * @param scale The scale value. Valid range is [0..127]
             * @return A reference to the key builder.
             */
            constexpr KeyBuilder& SetScale(uint8_t scale)
            {
                m_parent.m_values.Keys[static_cast<size_t>(m_key)].PulseAndScale.Scale = scale;

                return *this;
            }

        private:
            /**
             * @brief Constructor of a key builder.
             * @param parent Owner of the builder.
             * @param key The key to configure.
             */
            constexpr KeyBuilder(Builder& parent, AT24QT2120::Keys key)
            : m_parent(parent), m_key(key)
            {
            }

            //! Parent of this builder.
            Builder& m_parent;
            //! The key being configured.
            AT24QT2120::Keys m_key;
        };

    public:
        constexpr explicit Builder(Drivers::I2cModule::Handle* i2c) noexcept : m_i2c(i2c) {}
        constexpr ~Builder() noexcept = default;

        /**
         * @brief Uses the provided pin to monitor events coming from the touch sensor.
         * @param pin The pin to monitor
         * @return A reference to the builder.
         */
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
        /**
         * @brief Use the sensor in interrupt mode.
         *
         * This binds the specified event to the module.
         *
         * @attention For the sensor to work in interrupt mode, an external interrupt must be
         * configured on a GPIO, with its trigger set on the falling edge.
         *
         * @param type The EXTI used by the sensor.
         * @return A reference to the builder.
         *
         * @attention Interrupt mode is only available when @ref NILAI_USE_EVENTS is defined.
         */
        constexpr Self& UseInInterruptMode(Events::EventTypes type)
        {
            NILAI_ASSERT(Events::IsInCategory(type, Events::EventCategories::External),
                         "Event must be external!");

            m_irqType     = type;
            m_isInIrqMode = true;

            return *this;
        }
#    endif

        /**
         * Refer to @ref At24Qt2120::SetSamplingInterval for the documentation.
         */
        constexpr Self& SetSamplingInterval(size_t time)
        {
            m_values.SamplingInterval = time;

            return *this;
        }

        /**
         * Refer to @ref At24Qt2120::SetTowardTouchDrift for the documentation.
         */
        constexpr Self& SetTowardTouchDrift(uint8_t v)
        {
            m_values.TowardTouchDrift = v;

            return *this;
        }

        /**
         * Refer to @ref At24Qt2120::SetAwayFromTouchDrift for the documentation.
         */
        constexpr Self& SetAwayFromTouchDrift(uint8_t v)
        {
            m_values.AwayFromTouchDrift = v;

            return *this;
        }

        /**
         * Refer to @ref At24Qt2120::SetDetectionIntegrator for the documentation.
         */
        constexpr Self& SetDetectionIntegrator(uint8_t v)
        {
            // Detection integrator value ranges from 1 to 32.
            m_values.DetectionIntegrator = std::max(v, static_cast<uint8_t>(32));

            return *this;
        }

        /**
         * Refer to @ref At24Qt2120::SetTouchRecalibrationDelay for the documentation.
         */
        constexpr Self& SetTouchRecalibrationDelay(size_t delay)
        {
            m_values.TouchRecalibrationDelay = delay;

            return *this;
        }

        /**
         * Refer to @ref At24Qt2120::SetDriftHoldTime for the documentation.
         */
        constexpr Self& SetDriftHoldTime(size_t time)
        {
            m_values.DriftHoldTime = time;

            return *this;
        }

        /**
         * Refer to @ref At24Qt2120::SetSliderOptions for the documentation.
         */
        constexpr Self& SetSliderOptions(bool enable, bool wheel)
        {
            m_values.SliderOption = {enable, wheel};

            return *this;
        }

        /**
         * Refer to @ref At24Qt2120::SetChargeTime for the documentation.
         */
        constexpr Self& SetChargeTime(uint8_t us)
        {
            m_values.ChargeTime = us;

            return *this;
        }

        /**
         * @brief Creates a key builder for the specified key.
         * @param key The key to configure
         * @return The key builder for the requested key.
         */
        constexpr KeyBuilder SetKeyOptions(AT24QT2120::Keys key)
        {
            return KeyBuilder::Create(*this, key);
        }

        /**
         * @brief Builds and initialize the touch sensor with the configuration kept in the builder.
         * @return The instance of the touch sensor.
         */
        [[nodiscard]] At24Qt2120 Build() const;

    private:
        //! @brief Handles to the I2C peripheral used by the touch sensor.
        Drivers::I2cModule::Handle* m_i2c = nullptr;

        //! @brief Configuration values.
        AT24QT2120::RegisterMap m_values = {};

#    if defined(NILAI_USE_EVENTS)
        /**
         * @brief Flag keeping track of whether the touch sensor should be used in interrupt mode
         * or polling mode.
         *
         * @note This flag is not present if @ref NILAI_USE_EVENTS is not defined.
         */
        bool m_isInIrqMode = false;
        /**
         * @brief Event to which the touch sensor will be bound if used in interrupt mode.
         *
         * @note This member is not present if @ref NILAI_USE_EVENTS is not defined.
         */
        Events::EventTypes m_irqType = Events::EventTypes::Count;
#    endif

        /**
         * @brief Pin that will be monitored if the touch sensor is used in polling mode.
         */
        Pin m_pin = {};
    };

    /**
     * @brief Creates a builder for the touch sensor using the provided I2C peripheral.
     * @param i2c A pointer to the I2C peripheral to be used by the touch sensor.
     * @return The builder.
     */
    static constexpr Builder Create(Drivers::I2cModule::Handle* i2c) { return Builder(i2c); }

    /**
     * @brief Get the last time at which the touch sensor detected a change in its state.
     * @returns The timestamp of the last event, expressed in milliseconds since startup.
     */
    [[nodiscard]] uint32_t GetLastEventTime() const;

    bool DoPost() override;

    /**
     * @brief Run function of the module.
     *
     * When the touch sensor is operating in polling mode, @ref m_changePin is constantly monitored.
     * When a transition from HIGH to LOW is detected, the last event time (@ref m_lastEventTime)
     * is refreshed.
     */
    void Run() override;

    /**
     * @brief Gets the chip's ID.
     * @return Should always be 0x3E.
     */
    [[nodiscard]] uint8_t GetId() noexcept;

    /**
     * @brief Gets the firmware version of the chip.
     * @return The firmware version.
     */
    [[nodiscard]] AT24QT2120::FirmwareVersion GetFirmwareVersion() noexcept;

    /**
     * @brief Gets the status information from the chip.
     * @return The sensor's DetectionStatus, KeyStatus and SliderStatus.
     */
    [[nodiscard]] AT24QT2120::SensorStatus GetSensorStatus() noexcept;

    /**
     * @brief Gets the detection status of the chip.
     * @return The detection status.
     */
    [[nodiscard]] AT24QT2120::DetectionStatus GetDetectionStatus() noexcept;

    /**
     * @brief Gets the status of each keys.
     * @return The status of the keys.
     */
    [[nodiscard]] AT24QT2120::KeyStatus GetKeyStatus() noexcept;

    /**
     * @brief Gets the position of the slider/wheel.
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
     * @returns True if the sensor responded after the reset.
     * @returns False if the sensor failed to respond within the allowed time.
     *
     * @note If <b>waitForReset</b> is set, this method will block for up to 210ms.
     */
    bool Reset(bool waitForReset = false) noexcept;

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
    void SetSamplingInterval(size_t time) noexcept;

    /**
     * @brief Gets the interval between key measurements.
     * @return The sampling interval read from the sensor.
     */
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
    void SetTowardTouchDrift(uint8_t v) noexcept;

    /**
     * @brief Gets the Toward Touch Drift characteristic.
     * @return The Toward Touch Drift characteristic read from the sensor.
     */
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
    void SetAwayFromTouchDrift(uint8_t v) noexcept;

    /**
     * @brief Gets the Away From Touch Drift characteristic.
     * @return The Away From Touch Drift characteristic read from the sensor.
     */
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
    void SetDetectionIntegrator(uint8_t v) noexcept;

    /**
     * @brief Gets the current value for the Detection Integrator.
     * @return The Detection Integrator value read from the sensor.
     */
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
    void SetTouchRecalibrationDelay(size_t delay) noexcept;

    /**
     * @brief Gets the delay before a calibration is requested following a touch detection.
     * @return The delay read from the sensor.
     */
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
    void SetDriftHoldTime(size_t time) noexcept;

    /**
     * @brief Gets the drift hold time used by the sensor.
     * @return The hold time read from the chip.
     */
    [[nodiscard]] size_t GetDriftHoldTime() noexcept;

    /**
     * Sets the slider options.
     *
     * Defaults to disabled.
     *
     * @param opts Slider options to be used.
     */
    void SetSliderOptions(AT24QT2120::SliderOptions opts) noexcept
    {
        SetSliderOptions(opts.Enable, opts.Wheel);
    }

    /**
     * @brief Sets the slider options of the sensor.
     * @param enable Toggles the use of the slider channel.
     * @param wheel Toggles the use of a wheel instead of a slider.
     */
    void SetSliderOptions(bool enable, bool wheel) noexcept;

    /**
     * @brief Reads the current slider from the sensor.
     * @return The options read from the sensor.
     */
    [[nodiscard]] AT24QT2120::SliderOptions GetSliderOptions() noexcept;

    /**
     * @brief Prolongs the charge-transfer period of the signal acquisition by X microseconds.
     *
     * @param us The number of microseconds to prolong the acquisition by.
     */
    void SetChargeTime(uint8_t us) noexcept;

    /**
     * @brief Reads the current charge time from the sensor.
     * @return The current charge time read from the sensor, in microseconds.
     */
    [[nodiscard]] uint8_t GetChargeTime() noexcept;

    /**
     * @brief Sets the detection threshold for the specified key.
     *
     * Defaults to 10.
     *
     * @param key The key to set.
     * @param threshold The threshold of that key.
     *
     * @attention Do not use a setting of 0 as this causes a key to go into detection when its
     * signal is equal to its reference.
     */
    void SetDetectionThreshold(AT24QT2120::Keys key, uint8_t threshold) noexcept;

    /**
     * @brief Gets the current detection threshold for the specified key.
     * @param key The key to get the detection threshold from.
     * @return The detection threshold for the requested key.
     */
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
     * @param options The options for the key.
     */
    void SetKeyOptions(AT24QT2120::Keys key, const AT24QT2120::KeyOptions& options) noexcept;

    /**
     * @brief Get the options for the requested key.
     * @param key The key to get the options from.
     * @return The options for the requested key.
     */
    [[nodiscard]] AT24QT2120::KeyOptions GetKeyOptions(AT24QT2120::Keys key) noexcept;

    /**
     * @brief Refer to the datasheet for the exact details of modifying the Pulse/Scale of keys.
     *
     * Defaults to 0 for every key.
     *
     * @param key The key to operate on.
     * @param pulse The pulse value.
     */
    void SetKeyPulse(AT24QT2120::Keys key, uint8_t pulse) noexcept;

    /**
     * @brief Refer to the datasheet for the exact details of modifying the Pulse/Scale of keys.
     *
     * Defaults to 0 for every key.
     *
     * @param key The key to operate on.
     * @param scale The scale value.
     */
    void SetKeyScale(AT24QT2120::Keys key, uint8_t scale) noexcept;

    /**
     * @brief Sets the pulse and scale for the specified key.
     *
     * @note Refer to the datasheet for the exact details of modifying the Pulse/Scale of keys.
     *
     * @param key The key to set.
     * @param ps The pulse and scale value for the key.
     */
    void SetKeyPulseScale(AT24QT2120::Keys key, const AT24QT2120::PulseScale& ps) noexcept;

    /**
     * @brief Gets the pulse and scale values for the specified key.
     * @param key The key to get.
     * @return The key's pulse and scale values.
     */
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

    [[nodiscard]] AT24QT2120::SignalReferencePack GetSignalReferencePack() noexcept;

    /**
     * @brief Provided only as a mean to have un-initialized instances.
     * Using an un-initialized instance will result in UB.
     */
    constexpr At24Qt2120() noexcept = default;
    constexpr At24Qt2120(At24Qt2120&&) noexcept;
    At24Qt2120& operator=(At24Qt2120&& o) noexcept;

    constexpr operator bool() const noexcept { return m_initialized; }

private:
    /**
     * @brief Constructor that only initializes the under laying @ref I2cModule
     * @param i2c
     */
    constexpr explicit At24Qt2120(Drivers::I2cModule::Handle* i2c) noexcept
    : I2cModule(i2c, "AT24QT I2C"), m_run(&PollingRun)
    {
    }

    /**
     * @brief Function that is called by default in @ref Run.
     * @param self Pointer to the module.
     */
    static void DefaultRun([[maybe_unused]] At24Qt2120* self) {}

    /**
     * @brief Function that is called in @ref Run when the sensor is in polling mode.
     * @param self Pointer to the module.
     */
    static void PollingRun(At24Qt2120* self);

#    if defined(NILAI_USE_EVENTS)
    /**
     * @brief Function that is called in @ref Run when the sensor is in interrupt mode.
     * @param self Pointer to the module.
     *
     * @note @ref NILAI_USE_EVENTS must be defined for this method to exist.
     */
    static void IrqRun(At24Qt2120* self);

    /**
     * @brief Binds the touch sensor with the specified event.
     *
     * This registers the module in the @ref Application. @ref HandleIrq will be called whenever
     * the event occurs.
     *
     * @param type The event type. It must be an External Interrupt.
     *
     * @note @ref NILAI_USE_EVENTS must be defined for this method to exist.
     */
    void BindIrq(Events::EventTypes type);

    /**
     * @brief Callback function that gets called whenever the interrupt event occurs when the
     * sensor is in interrupt mode.
     *
     * @param e A pointer to the event.
     * @returns True if the event is the desired one.
     * @returns False for any other events.
     */
    bool HandleIrq(Events::Event* e);
#    endif

    /**
     * @brief Converts a value in milliseconds to the value that needs to be written in the
     * sensor's registers.
     *
     * The register value corresponds to the number of 16 ms.
     *
     * @param time The sampling time, in milliseconds.
     * @return The register value.
     */
    static constexpr uint8_t SamplingTimeToRegVal(size_t time) { return time / 16; }

    /**
     * @brief Converts a register value to the corresponding amount of time
     *
     * The register value corresponds to the number of 16 ms.
     *
     * @param regVal The value in the register.
     * @return The corresponding amount of time.
     */
    static constexpr size_t RegValToSamplingTime(uint8_t regVal) { return regVal * 16; }

    /**
     * @brief Converts a value in milliseconds to the value that needs to be written in the
     * sensor's registers.
     *
     * The register value corresponds to the number of 160 ms.
     *
     * @param time The sampling time, in milliseconds.
     * @return The register value.
     */
    static constexpr uint8_t DriftTimeToRegVal(size_t time) { return time / 160; }

    /**
     * @brief Converts a register value to the corresponding amount of time
     *
     * The register value corresponds to the number of 160 ms.
     *
     * @param regVal The value in the register.
     * @return The corresponding amount of time.
     */
    static constexpr size_t RegValToDriftTime(uint8_t regVal) { return regVal * 160; }

    /**
     * @brief Determines the exact register for a specific key.
     * @param key The desired key.
     * @param firstReg The register.
     * @return The register for the desired key.
     */
    static constexpr AT24QT2120::Registers RegisterFromKey(AT24QT2120::Keys      key,
                                                           AT24QT2120::Registers firstReg)
    {
        return static_cast<AT24QT2120::Registers>(
          (static_cast<size_t>(firstReg) + static_cast<size_t>(key)));
    }

    /**
     * @brief Reads a register from the sensor.
     * @param r The register to read.
     * @param cnt The number of bytes to read from the sensor.
     * @return The data read from the sensor.
     */
    I2C::Frame GetRegisters(AT24QT2120::Registers r, size_t cnt);

    /**
     * @brief Writes to a register in the sensor.
     * @param r The register to write to.
     * @param data A pointer to the data to write.
     * @param cnt The number of bytes to write.
     */
    void SetRegisters(AT24QT2120::Registers r, const uint8_t* data, size_t cnt);

    /**
     * @brief Waits for the sensor's calibration sequence to complete.
     * @param timeout The maximum amount of time that this function will wait for.
     * @returns True if the calibration was completed within the allowed amount of time.
     * @returns False if the timeout expired before the completion of the calibration.
     */
    bool WaitForCalibrationEnd(size_t timeout);

private:
    friend class At24Qt2120Builder;
    //! Function called in Run. This depends on the mode of operation, polling or interrupt-based.
    RunFunction m_run = &DefaultRun;

    //! Time of the last event detected by the touch sensor.
    uint32_t m_lastEventTime = 0;

    //! When in Polling mode, represents the pin to monitor.
    Pin m_changePin = {};

    //! Set to true by the builder, indicates a properly functioning chip.
    bool m_initialized = false;

#    if defined(NILAI_USE_EVENTS)
    /**
     * @brief     The ID of the event callback.
     * @note This member does not exist if @ref NILAI_USE_EVENTS is not defined.
     */
    size_t m_irqId = std::numeric_limits<size_t>::max();
    /**
     * @brief When in Interrupt mode, represents the event that triggers the IRQ.
     * @note This member does not exist if @ref NILAI_USE_EVENTS is not defined.
     */
    Events::EventTypes m_irqType = Events::EventTypes::Exti0;
#    endif

    static constexpr uint8_t s_i2cAddress = 0x1C << 1;
    static constexpr uint8_t s_chipId     = 0x3E;
};
/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */
}    // namespace Nilai::Interfaces

#endif


#endif    // GUARD_AT24QT2120_H
