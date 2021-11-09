/**
 ******************************************************************************
 * @addtogroup ledPattern
 * @{
 * @file    ledPattern
 * @author  Samuel Martel
 * @brief   Header for the ledPattern module.
 *
 * @date 9/23/2020 1:48:20 PM
 *
 ******************************************************************************
 */
#ifndef _ledPattern
#define _ledPattern

/*****************************************************************************/
/* Includes */
#include <vector>

namespace cep
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

}    // namespace cep
/* Have a wonderful day :) */
#endif /* _ledPattern */
/**
 * @}
 */
/****** END OF FILE ******/
