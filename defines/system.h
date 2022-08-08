/**
 * @file    system.h
 * @author  Samuel Martel
 * @date    2022-05-17
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

#ifndef NILAI_DEFINES_SYSTEM_H
#define NILAI_DEFINES_SYSTEM_H

#if defined(NILAI_TEST)
#    define NILAI_BREAKPOINT __asm("INT3")
#else
#    define NILAI_BREAKPOINT __asm("bkpt 1")
#endif

namespace Nilai::System
{
void Reset();
bool IsDebuggerConnected();
}    // namespace Nilai::System

#endif    // NILAI_DEFINES_SYSTEM_H
