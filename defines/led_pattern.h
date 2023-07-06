/**
 ******************************************************************************
 * @file    led_pattern.h
 * @author  Samuel Martel
 * @brief   Header for the ledPattern module.
 *
 * @date 9/23/2020 1:48:20 PM
 *
 ******************************************************************************
 */
#ifndef GUARD_LED_PATTERN_H
#    define GUARD_LED_PATTERN_H

/*****************************************************************************/
/* Includes */
#    include <cstddef>
#    include <vector>

namespace Nilai
{
/*****************************************************************************/
/* Exported defines */

/*****************************************************************************/
/* Exported macro */

/*****************************************************************************/
/* Exported types */
struct LedPattern
{
    size_t timeOn      = 0;
    size_t timeOff     = 0;
    int    repetitions = 0;
};

using LedSequence = std::vector<LedPattern>;

namespace StatusPatterns
{
constexpr LedPattern NoErrors {500, 500, -1};
constexpr LedPattern Error {100, 900, -1};
}    // namespace StatusPatterns

/*****************************************************************************/
/* Exported functions */

}    // namespace Nilai
/* Have a wonderful day :) */
#endif /* GUARD_LED_PATTERN_H */
/****** END OF FILE ******/
