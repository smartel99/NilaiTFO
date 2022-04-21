/**
 * @file    Literals.h
 * @author  Samuel Martel
 * @date    2022-04-21
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
#ifndef NILAI_LITERALS_H
#define NILAI_LITERALS_H

#include <cstdint>

uint8_t operator "" _u8(unsigned long long int i)
{
    return static_cast<uint8_t>(i);
}

int8_t operator "" _s8(unsigned long long int i)
{
    return static_cast<int8_t>(i);
}

uint16_t operator "" _u16(unsigned long long int i)
{
    return static_cast<uint16_t>(i);
}

int16_t operator "" _s16(unsigned long long int i)
{
    return static_cast<int16_t>(i);
}

uint32_t operator "" _u32(unsigned long long int i)
{
    return static_cast<uint32_t>(i);
}

int32_t operator "" _s32(unsigned long long int i)
{
    return static_cast<int32_t>(i);
}


#endif    // NILAI_LITERALS_H
