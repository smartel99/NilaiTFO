/**
 * @addtogroup core.h
 * @{
 *******************************************************************************
 * @file	core.h
 * @author	Samuel Martel
 * @brief
 * Created on: Apr. 21, 2021
 *******************************************************************************
 */

#ifndef CORE_H_
#define CORE_H_

#include <memory>

namespace cep
{
/**
 * @brief   Alias for std::shared_ptr
 *
 * @tparam T The type of the pointer.
 */
template<typename T>
using Ref = std::shared_ptr<T>;

/**
 * @brief   Create a reference object
 *
 * @tparam T The type of object to create
 * @tparam Args
 * @param   args Arguments to pass to the constructor of the object
 * @retval  constexpr Ref<T> The contructed
 */
template<typename T, typename... Args>
constexpr Ref<T> CreateRef(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}


/**
 * @brief   Checks if the pointer is null or not.
 *
 * @tparam T
 * @param   ptr
 * @retval  true
 * @retval  false
 */
template<typename T>
constexpr bool IsPointerNotNull(T* ptr)
{
    return ptr != nullptr;
}

template<typename T>
constexpr bool IsPointerNotNull(const T* ptr)
{
    return ptr != nullptr;
}

}    // namespace cep


/**
 * @brief   BLX(1) is not supported by the Cortex M3 CPU, but its behavior if used is still
 *          defined: it always causes a hard fault, which is exactly what we want!
 */
constexpr void Crash()
{
    __asm__("BLX(1)");
}

/**
 * @}
 */
/* END OF FILE */
#endif /* CORE_H_ */
