/**
 * @file    enums.h
 * @author  Samuel Martel
 * @date    2022-05-02
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
#ifndef NILAI_RTC_ENUMS_H
#define NILAI_RTC_ENUMS_H

#if defined(NILAI_USE_RTC)
namespace Nilai::Rtc
{
enum class DayLightSaving
{
    None   = 0,
    AddOne = 1,
    SubOne = 2
};
}
#endif

#endif    // NILAI_RTC_ENUMS_H
