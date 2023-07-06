/**
 * @file	version.h
 * @author	Samuel Martel
 * @brief
 * Created on: Jul 8, 2021
 *******************************************************************************
 */

#ifndef VERSION_H_
#define VERSION_H_

/***********************************************/
/* Includes */
#include <cstdint>

/***********************************************/
/* Defines */

/***********************************************/
/* Function declarations */

namespace Nilai
{
static constexpr size_t GetMonthFromDate()
{
    return (__DATE__[0] == 'J')
             ? ((__DATE__[1] == 'a')
                  ? 0x00000100
                  : ((__DATE__[2] == 'n') ? 0x00000600 : 0x00000700))    // Jan, Jun or Jul
           : (__DATE__[0] == 'F') ? 0x00000200                           // Feb
           : (__DATE__[0] == 'M')
             ? ((__DATE__[2] == 'r') ? 0x00000300 : 0x00000500)    // Mar or May
           : (__DATE__[0] == 'A')
             ? ((__DATE__[2] == 'p') ? 0x00000400 : 0x00000800)    // Apr or Aug
           : (__DATE__[0] == 'S') ? 0x00000900                     // Sep
           : (__DATE__[0] == 'O') ? 0x00001000                     // Oct
           : (__DATE__[0] == 'N') ? 0x00001100                     // Nov
           : (__DATE__[0] == 'D') ? 0x00001200                     // Dec
                                  : 0x00000000;
}

/**
 * Gets the firmware version.
 * Formatted as YYYYMMDD, in binary coded decimal.
 * Example: Jul 8th 2021 will return 0x20210708
 * @return
 */
static constexpr size_t GetVersion()
{
    // __DATE__ gives a char* formatted as "MMM DD YYYY"
    size_t ver = GetMonthFromDate();
    ver |= (size_t)(__DATE__[7] - '0') << 28;    // Most significant digit of the year
    ver |= (size_t)(__DATE__[8] - '0') << 24;
    ver |= (size_t)(__DATE__[9] - '0') << 20;
    ver |= (size_t)(__DATE__[10] - '0') << 16;    // Least significant digit of the year.

    ver |= (size_t)(__DATE__[4] == ' ' ? 0 : (__DATE__[4] - '0'))
           << 4;                           // Most significant digit of the day.
    ver |= (size_t)(__DATE__[5] - '0');    // Least significant digit of the day.

    return ver;
}
}    // namespace Nilai

/* END OF FILE */
#endif /* VERSION_H_ */
