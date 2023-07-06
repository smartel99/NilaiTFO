/**
 * @file    map.h
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

#ifndef GUARD_AT24QT2120_REGISTERS_MAP_H
#define GUARD_AT24QT2120_REGISTERS_MAP_H

#if defined(NILAI_USE_AT24QT2120)
#    include <cstdint>

namespace Nilai::Interfaces::AT24QT2120
{
enum class Registers : uint8_t
{
    ChipId = 0,
    FirmwareVersion,
    DetectionStatus,
    KeyStatus1,
    KeyStatus2,
    SliderPosition,
    Calibrate,
    Reset,
    LowPowerMode,
    TowardTouchDrift,
    AwayFromTouchDrift,
    DetectionIntegrator,
    TouchRecalDelay,
    DriftHoldTime,
    SliderOption,
    ChargeTime,
    Key0DetectThreshold,
    Key1DetectThreshold,
    Key2DetectThreshold,
    Key3DetectThreshold,
    Key4DetectThreshold,
    Key5DetectThreshold,
    Key6DetectThreshold,
    Key7DetectThreshold,
    Key8DetectThreshold,
    Key9DetectThreshold,
    Key10DetectThreshold,
    Key11DetectThreshold,
    Key0Control,
    Key1Control,
    Key2Control,
    Key3Control,
    Key4Control,
    Key5Control,
    Key6Control,
    Key7Control,
    Key8Control,
    Key9Control,
    Key10Control,
    Key11Control,
    Key0PulseScale,
    Key1PulseScale,
    Key2PulseScale,
    Key3PulseScale,
    Key4PulseScale,
    Key5PulseScale,
    Key6PulseScale,
    Key7PulseScale,
    Key8PulseScale,
    Key9PulseScale,
    Key10PulseScale,
    Key11PulseScale,
    KeySignal0MSB,
    KeySignal0LSB,
    KeySignal1MSB,
    KeySignal1LSB,
    KeySignal2MSB,
    KeySignal2LSB,
    KeySignal3MSB,
    KeySignal3LSB,
    KeySignal4MSB,
    KeySignal4LSB,
    KeySignal5MSB,
    KeySignal5LSB,
    KeySignal6MSB,
    KeySignal6LSB,
    KeySignal7MSB,
    KeySignal7LSB,
    KeySignal8MSB,
    KeySignal8LSB,
    KeySignal9MSB,
    KeySignal9LSB,
    KeySignal10MSB,
    KeySignal10LSB,
    KeySignal11MSB,
    KeySignal11LSB,
    ReferenceData0MSB,
    ReferenceData0LSB,
    ReferenceData1MSB,
    ReferenceData1LSB,
    ReferenceData2MSB,
    ReferenceData2LSB,
    ReferenceData3MSB,
    ReferenceData3LSB,
    ReferenceData4MSB,
    ReferenceData4LSB,
    ReferenceData5MSB,
    ReferenceData5LSB,
    ReferenceData6MSB,
    ReferenceData6LSB,
    ReferenceData7MSB,
    ReferenceData7LSB,
    ReferenceData8MSB,
    ReferenceData8LSB,
    ReferenceData9MSB,
    ReferenceData9LSB,
    ReferenceData10MSB,
    ReferenceData10LSB,
    ReferenceData11MSB,
    ReferenceData11LSB,
};
}
#endif

#endif    // GUARD_AT24QT2120_REGISTERS_MAP_H
