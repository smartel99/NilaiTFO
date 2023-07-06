/**
 * @file    validation.h
 * @author  Samuel Martel
 * @date    2022-09-23
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

#ifndef GUARD_NILAI_DEFINES_CONCEPTS_VALIDATION_H
#define GUARD_NILAI_DEFINES_CONCEPTS_VALIDATION_H

/**
 * @addtogroup Nilai
 * @{
 */

/**
 * @addtogroup Concepts
 * @{
 */

#include "internal.h"

#define NILAI_CALLABLE_CONCEPT(Concept)                                                            \
    []<typename T>() consteval                                                                     \
    {                                                                                              \
        return Concept<T>;                                                                         \
    }

#define NILAI_CALL_CONCEPT_WITH_TYPE(Concept, type)                                                \
    NILAI_CALLABLE_CONCEPT(Concept).template operator()<type>()

#define NILAI_CONCEPT_EXPECT_TRUE_WITH_TYPE(Concept, type)                                         \
    static_assert(NILAI_CALL_CONCEPT_WITH_TYPE(Concept, type))

#define NILAI_CONCEPT_EXPECT_FALSE_WITH_TYPE(Concept, type)                                        \
    static_assert(!NILAI_CALL_CONCEPT_WITH_TYPE(Concept, type))

#define NILAI_CONCEPT_EXPECT_X_WITH_TYPES_HELPER(Concept, expected, line, ...)                     \
    static_assert(std::same_as<decltype(expected), bool>,                                          \
                  "expected must be either true or false!");                                       \
                                                                                                   \
    static_assert(__VA_OPT__(true ||) false, "Types Needed!");                                     \
                                                                                                   \
    __VA_OPT__(namespace Nilai::Internal {                                                         \
        template<typename T, typename... Ts>                                                       \
        consteval void NILAI_INTERNAL_CONCEPT_LABEL(Concept, ExpectXWithTypes, expected, line)()   \
        {                                                                                          \
            static_assert(Concept<T> == expected);                                                 \
            if constexpr (sizeof...(Ts))                                                           \
            {                                                                                      \
                NILAI_INTERNAL_CONCEPT_LABEL(Concept, ExpectXWithTypes, expected, line)<Ts...>();  \
            }                                                                                      \
        }                                                                                          \
        static_assert((NILAI_INTERNAL_CONCEPT_LABEL(Concept, ExpectXWithTypes, expected, line) <   \
                         __VA_ARGS__ > (),                                                         \
                       true),                                                                      \
                      "Concept does not work with types!");                                        \
    })

#define NILAI_CONCEPT_EXPECT_X_WITH_TYPES(Concept, expected, ...)                                  \
    NILAI_CONCEPT_EXPECT_X_WITH_TYPES_HELPER(Concept, expected, __LINE__ __VA_OPT__(, ) __VA_ARGS__)

#define NILAI_CONCEPT_ACCEPTS_TYPES(Concept, ...)                                                  \
    NILAI_CONCEPT_EXPECT_X_WITH_TYPES(Concept, true __VA_OPT__(, ) __VA_ARGS__)

#define NILAI_CONCEPT_REJECTS_TYPES(Concept, ...)                                                  \
    NILAI_CONCEPT_EXPECT_X_WITH_TYPES(Concept, false __VA_OPT__(, ) __VA_ARGS__)

#define NILAI_CONCEPT_ALL_PRIMITIVES()                                                             \
    bool, char, short, int, long, long long, unsigned char, unsigned short, unsigned int,          \
      unsigned long, unsigned long long, float, double

#define NILAI_CONCEPT_ALL_PRIMITIVES_OF_CONTAINER(x, ...)                                          \
    x<bool __VA_OPT__(, ) __VA_ARGS__>, x<char __VA_OPT__(, ) __VA_ARGS__>,                        \
      x<short __VA_OPT__(, ) __VA_ARGS__>, x<int __VA_OPT__(, ) __VA_ARGS__>,                      \
      x<long __VA_OPT__(, ) __VA_ARGS__>, x<long long __VA_OPT__(, ) __VA_ARGS__>,                 \
      x<unsigned char __VA_OPT__(, ) __VA_ARGS__>, x<unsigned short __VA_OPT__(, ) __VA_ARGS__>,   \
      x<unsigned int __VA_OPT__(, ) __VA_ARGS__>, x<unsigned long __VA_OPT__(, ) __VA_ARGS__>,     \
      x<unsigned long long __VA_OPT__(, ) __VA_ARGS__>, x<float __VA_OPT__(, ) __VA_ARGS__>,       \
      x<double __VA_OPT__(, ) __VA_ARGS__>

//!@}
//!@}

#endif    // GUARD_NILAI_DEFINES_CONCEPTS_VALIDATION_H
