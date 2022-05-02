/**
 * @addtogroup Core.h
 * @{
 *******************************************************************************
 * @file	Core.h
 * @author	Samuel Martel
 * @brief
 * Created on: Apr. 21, 2021
 *******************************************************************************
 */

#ifndef NILAI_CORE_H_
#define NILAI_CORE_H_

#include <memory>

namespace cep
{
template<typename T>
using Ref = std::shared_ptr<T>;

template<typename T, typename... Args>
constexpr Ref<T> CreateRef(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}
}    // namespace cep




/**
 * BLX(1) is not supported by the Cortex M3 CPU, but its behavior if used is still
 * defined: it always causes a hard fault, which is exactly what we want!
 */
#define CRASH __asm__("BLX(1)")

/**
 * @}
 */
/* END OF FILE */
#endif /* NILAI_CORE_H_ */
