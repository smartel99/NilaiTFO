/**
 * @file    serializer.cpp
 * @author  Samuel Martel
 * @date    2022-09-29
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
#include <gtest/gtest.h>

#include "defines/concepts/validation.h"
#include "services/serializer.h"

#include <array>
#include <string>
#include <type_traits>
#include <vector>


template<typename T, size_t N>
struct MeetsAllReqs
{
    using value_type     = T;
    using iterator       = value_type*;
    using const_iterator = const value_type*;
    using size_type      = size_t;

    value_type data[N] = {};

    iterator       begin() { return data; }
    const_iterator begin() const { return data; }
    iterator       end() { return data + N; }
    const_iterator end() const { return data + N; }
    size_type      size() const { return N; }
};

TEST(NilaiSerializer, SerializesPrimitives)
{
    EXPECT_EQ(Nilai::Serialize((uint8_t)0xAA), std::vector<uint8_t> {0xAA});
    EXPECT_EQ(Nilai::Serialize((int8_t)0x55), std::vector<uint8_t> {0x55});
    EXPECT_EQ(Nilai::Serialize((uint16_t)0x1122), (std::vector<uint8_t> {0x11, 0x22}));
    EXPECT_EQ(Nilai::Serialize((int16_t)0x1122), (std::vector<uint8_t> {0x11, 0x22}));
    EXPECT_EQ(Nilai::Serialize((uint32_t)0x11223344),
              (std::vector<uint8_t> {0x11, 0x22, 0x33, 0x44}));
    EXPECT_EQ(Nilai::Serialize((int32_t)0x11223344),
              (std::vector<uint8_t> {0x11, 0x22, 0x33, 0x44}));
    EXPECT_EQ(Nilai::Serialize((uint64_t)0x1122334455667788),
              (std::vector<uint8_t> {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88}));
    EXPECT_EQ(Nilai::Serialize((int64_t)0x1122334455667788),
              (std::vector<uint8_t> {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88}));
    EXPECT_EQ(Nilai::Serialize(1.234f), (std::vector<uint8_t> {0x3f, 0x9d, 0xf3, 0xb6}));
    EXPECT_EQ(Nilai::Serialize(1.234),
              (std::vector<uint8_t> {0x3F, 0xF3, 0xBE, 0x76, 0xC8, 0xB4, 0x39, 0x58}));
}

TEST(NilaiSerializer, SerializesStandardContainers)
{
    EXPECT_EQ(Nilai::Serialize(std::string {"asdf"}), (std::vector<uint8_t> {'a', 's', 'd', 'f'}));
    EXPECT_EQ((Nilai::Serialize(std::vector<int> {0x11223344, 0x55667788})),
              (std::vector<uint8_t> {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88}));
    EXPECT_EQ((Nilai::Serialize(std::array {0x11223344, 0x55667788})),
              (std::vector<uint8_t> {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88}));
}

TEST(NilaiSerializer, SerializeUserDefinedClass)
{
    struct ToVector
    {
        uint64_t v;
                 operator std::vector<uint8_t>() const
        {
            return {static_cast<uint8_t>(v >> 56),
                    static_cast<uint8_t>(v >> 48),
                    static_cast<uint8_t>(v >> 40),
                    static_cast<uint8_t>(v >> 32),
                    static_cast<uint8_t>(v >> 24),
                    static_cast<uint8_t>(v >> 16),
                    static_cast<uint8_t>(v >> 8),
                    static_cast<uint8_t>(v)};
        }
    };
    EXPECT_EQ(Nilai::Serialize(ToVector {0x1122334455667788}),
              (std::vector<uint8_t> {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88}));

    EXPECT_EQ((Nilai::Serialize(MeetsAllReqs<uint16_t, 3> {0x1122, 0x3344, 0x5566})),
              (std::vector<uint8_t> {0x11, 0x22, 0x33, 0x44, 0x55, 0x66}));
}

using Nilai::Serializable;

struct ConvertibleToVector
{
    operator std::vector<uint8_t>() const { return {}; }
};

NILAI_CONCEPT_ACCEPTS_TYPES(Serializable,
                            NILAI_CONCEPT_ALL_PRIMITIVES(),
                            NILAI_CONCEPT_ALL_PRIMITIVES_OF_CONTAINER(std::vector),
                            NILAI_CONCEPT_ALL_PRIMITIVES_OF_CONTAINER(std::array, 1),
                            std::string,
                            std::string_view,
                            ConvertibleToVector,
                            MeetsAllReqs<uint16_t, 3>);

struct NotSerializable
{
};

struct BadValueType
{
    using value_type = std::map<std::string, std::string>;
    using iterator   = typename value_type::iterator;
    using size_type  = size_t;

    iterator  begin() { return {}; }
    iterator  end() { return {}; }
    size_type size() { return 0; }
};
struct NoValueType
{
    using iterator  = int*;
    using size_type = size_t;

    iterator  begin() { return nullptr; }
    iterator  end() { return nullptr; }
    size_type size() { return 0; }
};

struct NoIteratorType
{
    using value_type = std::vector<int>;
    using size_type  = size_t;

    value_type* begin() { return nullptr; }
    value_type* end() { return nullptr; }
    size_type   size() { return 0; }
};
struct BadBegin
{
    using value_type = uint8_t;
    using iterator   = value_type*;
    using size_type  = size_t;

    int*      begin() { return nullptr; }
    iterator  end() { return nullptr; }
    size_type size() { return 0; }
};
struct NoBegin
{
    using value_type = uint8_t;
    using iterator   = value_type*;
    using size_type  = size_t;

    iterator  end() { return nullptr; }
    size_type size() { return 0; }
};
struct BadEnd
{
    using value_type = uint8_t;
    using iterator   = value_type*;
    using size_type  = size_t;

    iterator  begin() { return nullptr; }
    int*      end() { return nullptr; }
    size_type size() { return 0; }
};
struct NoEnd
{
    using value_type = uint8_t;
    using iterator   = value_type*;
    using size_type  = size_t;

    iterator  begin() { return nullptr; }
    size_type size() { return 0; }
};
struct BadSize
{
    using value_type = uint8_t;
    using iterator   = value_type*;
    using size_type  = size_t;

    iterator begin() { return nullptr; }
    iterator end() { return nullptr; }
    int      size() { return 0; }
};
struct NoSize
{
    using value_type = uint8_t;
    using iterator   = value_type*;
    using size_type  = size_t;

    iterator begin() { return nullptr; }
    iterator end() { return nullptr; }
};

NILAI_CONCEPT_REJECTS_TYPES(Serializable,
                            NotSerializable,
                            BadValueType,
                            NoValueType,
                            NoIteratorType,
                            BadBegin,
                            NoBegin,
                            BadEnd,
                            NoEnd,
                            BadSize,
                            NoSize);
