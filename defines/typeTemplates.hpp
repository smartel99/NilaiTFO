/**
 * @addtogroup defines
 * @{
 * @addtogroup typeTemplates
 * @{
 * @file    typeTemplates.hpp
 * @author  Pascal-Emmanuel Lachance
 * @p       https://www.github.com/Raesangur
 * @date    2020/08/10 -- 09:03
 *
 * @brief   Header file for small templated typedefs.
 */
#pragma once
/*************************************************************************************************/
/* Includes ------------------------------------------------------------------------------------ */
#include <array>
#include <cstdint>
#include <string>
#include <type_traits>
#include <vector>


namespace cep
{


/*************************************************************************************************/
/* Type definitions ---------------------------------------------------------------------------- */

/*------------------------------------*/
/**
 * @defgroup width2
 * @brief    An integral type that is twice the size of the inputed integral type.
 *
 * From: https://stackoverflow.com/a/46385939
 */
#pragma region width2
template<typename>
struct width2;

template<>
struct width2<std::uint8_t>
{
    using type = std::uint16_t;
};

template<>
struct width2<std::uint16_t>
{
    using type = std::uint32_t;
};

template<>
struct width2<std::uint32_t>
{
    using type = std::uint64_t;
};

template<>
struct width2<std::int8_t>
{
    using type = std::int16_t;
};

template<>
struct width2<std::int16_t>
{
    using type = std::int32_t;
};

template<>
struct width2<std::int32_t>
{
    using type = std::int64_t;
};

/**
 * @}
 */
#pragma endregion


/*------------------------------------*/
/**
 * @defgroup is_std_vector
 * @brief    Check if a variable is of type std::vector
 */
#pragma region is_std_vector
template<typename>
struct is_std_vector : std::false_type
{
};

template<typename T, typename A>
struct is_std_vector<std::vector<T, A>> : std::true_type
{
};
/**
 * @}
 */
#pragma endregion


/*------------------------------------*/
/**
 * @defgroup is_std_array
 * @brief    Check if a variable is of type std::array
 */
#pragma region is_std_array
template<typename T>
struct is_std_array : std::false_type
{
};

template<std::size_t size>
struct is_std_array<std::array<char, size>> : std::true_type
{
};

template<std::size_t size>
struct is_std_array<std::array<uint8_t, size>> : std::true_type
{
};

template<std::size_t size>
struct is_std_array<std::array<uint16_t, size>> : std::true_type
{
};

template<std::size_t size>
struct is_std_array<std::array<uint32_t, size>> : std::true_type
{
};

template<std::size_t size>
struct is_std_array<std::array<uint64_t, size>> : std::true_type
{
};

template<std::size_t size>
struct is_std_array<std::array<std::string, size>> : std::true_type
{
};
/**
 * @}
 */
#pragma endregion


/*------------------------------------*/
/**
 * @defgroup is_bool
 * @brief    Check if a variable is of type bool
 */
#pragma region is_bool
template<typename T>
struct is_bool_helper : std::false_type
{
};

template<>
struct is_bool_helper<bool> : std::true_type
{
};

template<typename T>
struct is_bool : is_bool_helper<std::remove_cv_t<T>>
{
};
/**
 * @}
 */
#pragma endregion


/*------------------------------------*/
/**
 * @defgroup is_string
 * @brief    Check if a variable is of type std::string
 */
#pragma region is_string
template<typename T>
struct is_string_helper : std::false_type
{
};

template<>
struct is_string_helper<std::string> : std::true_type
{
};

template<typename T>
struct is_string : is_string_helper<std::remove_cv_t<T>>
{
};
/**
 * @}
 */
#pragma endregion


/*************************************************************************************************/
/* Type utilities ------------------------------------------------------------------------------ */

/**
 * @brief   Converts a strongly-typed enum class into an int (or specified underlying type).
 * 
 * From:    https://stackoverflow.com/a/33083231
 */
template<typename Enum>
constexpr std::underlying_type_t<Enum> Underlying(Enum e) noexcept
{
    return static_cast<std::underlying_type_t<Enum>>(e);
}


/*************************************************************************************************/
}    // namespace cep
/**
 * @}
 * @}
 */
/* ----- END OF FILE ----- */
