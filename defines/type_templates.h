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
/**
 * @addtogroup Nilai
 * @{
 */

/**
 * @addtogroup type_traits Type Traits
 * @{
 */

/*************************************************************************************************/
/* Type definitions ---------------------------------------------------------------------------- */

/*------------------------------------*/
/**
 * @defgroup width2 width2
 * @{
 * @brief    An integral type that is twice the size of the input integral type.
 *
 * From: https://stackoverflow.com/a/46385939
 */
/**
 * @brief Default, un-defined struct for invalid types.
 */
template<typename>
struct width2;

/**
 * @brief Converts an uint8_t into an uint16_t.
 */
template<>
struct width2<std::uint8_t>
{
    using type = std::uint16_t;
};

/**
 * @brief Converts an uint16_t into an uint32_t.
 */
template<>
struct width2<std::uint16_t>
{
    using type = std::uint32_t;
};

/**
 * @brief Converts an uint32_t into an uint64_t.
 */
template<>
struct width2<std::uint32_t>
{
    using type = std::uint64_t;
};

/**
 * @brief Converts an int8_t into an int16_t.
 */
template<>
struct width2<std::int8_t>
{
    using type = std::int16_t;
};

/**
 * @brief Converts an int16_t into an int32_t.
 */
template<>
struct width2<std::int16_t>
{
    using type = std::int32_t;
};

/**
 * @brief Converts an int32_t into an int64_t.
 */
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
 * @defgroup is_std_vector is_std_vector
 * @{
 * @brief    Check if a variable is of type std::vector
 */
/**
 * @brief Used when the type is not an std::vector.
 */
template<typename>
struct is_std_vector : std::false_type
{
};

/**
 * @brief Used when the type is an std::vector.
 * @tparam T Type of the vector.
 * @tparam A Allocator of the vector.
 */
template<typename T, typename A>
struct is_std_vector<std::vector<T, A>> : std::true_type
{
};
/**
 * @}
 */


/*------------------------------------*/
/**
 * @defgroup is_std_array is_std::array
 * @{
 * @brief    Check if a variable is of type std::array
 */

/**
 * @brief Used when the type is not an std::array
 */
template<typename>
struct is_std_array : std::false_type
{
};

/**
 * @brief Used when the type is an std::array<char, size>.
 * @tparam size The size of the array.
 */
template<std::size_t size>
struct is_std_array<std::array<char, size>> : std::true_type
{
};

/**
 * @brief Used when the type is an std::array<uint8_t, size>.
 * @tparam size The size of the array.
 */
template<std::size_t size>
struct is_std_array<std::array<uint8_t, size>> : std::true_type
{
};

/**
 * @brief Used when the type is an std::array<uint16_t, size>.
 * @tparam size The size of the array.
 */
template<std::size_t size>
struct is_std_array<std::array<uint16_t, size>> : std::true_type
{
};

/**
 * @brief Used when the type is an std::array<uint32_t, size>.
 * @tparam size The size of the array.
 */
template<std::size_t size>
struct is_std_array<std::array<uint32_t, size>> : std::true_type
{
};

/**
 * @brief Used when the type is an std::array<uint64_t, size>.
 * @tparam size The size of the array.
 */
template<std::size_t size>
struct is_std_array<std::array<uint64_t, size>> : std::true_type
{
};

/**
 * @brief Used when the type is an std::array<std::string, size>.
 * @tparam size The size of the array.
 */
template<std::size_t size>
struct is_std_array<std::array<std::string, size>> : std::true_type
{
};
/**
 * @}
 */


/*------------------------------------*/
/**
 * @defgroup is_bool is_bool
 * @{
 * @brief    Check if a variable is of type bool
 */
/**
 * @brief Type is not a bool
 */
template<typename>
struct is_bool_helper : std::false_type
{
};

/**
 * @brief Type is a bool
 */
template<>
struct is_bool_helper<bool> : std::true_type
{
};

/**
 * @brief Checks whether T is a bool or not.
 * @tparam T The type to check.
 */
template<typename T>
struct is_bool : is_bool_helper<std::remove_cv_t<T>>
{
};
/**
 * @}
 */


/*------------------------------------*/
/**
 * @defgroup is_string is_string
 * @{
 * @brief    Check if a variable is of type std::string
 */

/**
 * @brief Type is not a std::string.
 */
template<typename>
struct is_string_helper : std::false_type
{
};

/**
 * @brief Type is a std::string.
 */
template<>
struct is_string_helper<std::string> : std::true_type
{
};

/**
 * @brief Checks whether T is a std::string or not.
 * @tparam T The type to check.
 */
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
 * @tparam Enum The enum type
 * @param e The enumerated value
 * @return The corresponding integral value for the enumerated value
 */
template<typename Enum>
constexpr std::underlying_type_t<Enum> Underlying(Enum e) noexcept
{
    return static_cast<std::underlying_type_t<Enum>>(e);
}

/**
 * @}
 */

/**
 * @}
 */

/*************************************************************************************************/
}    // namespace Nilai
#endif
/* ----- END OF FILE ----- */
