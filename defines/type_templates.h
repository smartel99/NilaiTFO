/**
 * @file    type_templates.h
 * @author  Pascal-Emmanuel Lachance
 * @date    2020/08/10 -- 09:03
 *
 * @brief   Header file for small templated typedefs.
 */
#ifndef NILAI_TYPE_TEMPLATES_H
#    define NILAI_TYPE_TEMPLATES_H
/*************************************************************************************************/
/* Includes ------------------------------------------------------------------------------------ */
#    include <array>
#    include <cstdint>
#    include <string>
#    include <type_traits>
#    include <vector>


namespace Nilai
{


/*************************************************************************************************/
/* Type definitions ---------------------------------------------------------------------------- */

/*------------------------------------*/
/**
 * @defgroup width2 Integral type double the size of the input
 * @{
 * @brief    An integral type that is twice the size of the input integral type.
 *
 * From: https://stackoverflow.com/a/46385939
 */
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


/*------------------------------------*/
/**
 * @defgroup is_std_vector Is std::vector
 * @{
 * @brief    Check if a variable is of type std::vector
 */
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


/*------------------------------------*/
/**
 * @defgroup is_std_array Is std::array
 * @{
 * @brief    Check if a variable is of type std::array
 */
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


/*------------------------------------*/
/**
 * @defgroup is_bool Is bool
 * @{
 * @brief    Check if a variable is of type bool
 */
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


/*------------------------------------*/
/**
 * @defgroup is_string Is std::string
 * @{
 * @brief    Check if a variable is of type std::string
 */
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
}    // namespace Nilai
#endif
/* ----- END OF FILE ----- */
