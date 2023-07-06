/**
 * @file    deserializer.cpp
 * @author  Samuel Martel
 * @date    2022-10-11
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
#include "services/deserializer.h"

#include <cassert>

TEST(NilaiDeserializer, DeserializePrimitives)
{
    using namespace std::string_literals;
    using V = std::vector<uint8_t>;
    EXPECT_EQ(Nilai::Deserialize<uint8_t>(V {0x11}), 0x11);
    EXPECT_EQ(Nilai::Deserialize<int8_t>(V {0x55}), 0x55);
    EXPECT_EQ(Nilai::Deserialize<uint16_t>(V {0x11, 0x22}), 0x1122);
    EXPECT_EQ(Nilai::Deserialize<int16_t>(V {0x11, 0x22}), 0x1122);
    EXPECT_EQ(Nilai::Deserialize<uint32_t>(V {0x11, 0x22, 0x33, 0x44}), 0x11223344);
    EXPECT_EQ(Nilai::Deserialize<int32_t>(V {0x11, 0x22, 0x33, 0x44}), 0x11223344);
    EXPECT_EQ(Nilai::Deserialize<uint64_t>(V {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88}),
              0x1122334455667788);
    EXPECT_EQ(Nilai::Deserialize<int64_t>(V {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88}),
              0x1122334455667788);
    EXPECT_EQ(Nilai::Deserialize<float>(V {0x3F, 0x9D, 0xF3, 0xB6}), 1.234f);
    EXPECT_EQ(Nilai::Deserialize<double>(V {0x3F, 0xF3, 0xBE, 0x76, 0xC8, 0xB4, 0x39, 0x58}),
              1.234);
    EXPECT_EQ(Nilai::Deserialize<std::string>(V {'a', 's', 'd', 'f'}), "asdf"s);
}

TEST(NilaiDeserializer, DeserializeWithMissingData)
{
    using V = std::vector<uint8_t>;
    EXPECT_EQ(Nilai::Deserialize<uint8_t>(V {0x11}), 0x11);
    EXPECT_EQ(Nilai::Deserialize<int8_t>(V {0x55}), 0x55);
    EXPECT_EQ(Nilai::Deserialize<uint16_t>(V {0x11}), 0x1100);
    EXPECT_EQ(Nilai::Deserialize<int16_t>(V {0x11}), 0x1100);
    EXPECT_EQ(Nilai::Deserialize<uint32_t>(V {0x11, 0x22}), 0x11220000);
    EXPECT_EQ(Nilai::Deserialize<int32_t>(V {0x11, 0x22}), 0x11220000);
    EXPECT_EQ(Nilai::Deserialize<uint64_t>(V {0x11, 0x22, 0x33, 0x44}), 0x1122334400000000);
    EXPECT_EQ(Nilai::Deserialize<int64_t>(V {0x11, 0x22, 0x33, 0x44}), 0x1122334400000000);
}

TEST(NilaiDeserializer, DeserializeWithExtraData)
{
    using V = std::vector<uint8_t>;
    EXPECT_EQ(Nilai::Deserialize<uint8_t>(V {0x11, 0x22}), 0x11);
    EXPECT_EQ(Nilai::Deserialize<int8_t>(V {0x55, 0x11}), 0x55);
    EXPECT_EQ(Nilai::Deserialize<uint16_t>(V {0x11, 0x22, 0x33, 0x44}), 0x1122);
    EXPECT_EQ(Nilai::Deserialize<int16_t>(V {0x11, 0x22, 0x33, 0x44}), 0x1122);
    EXPECT_EQ(Nilai::Deserialize<uint32_t>(V {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88}),
              0x11223344);
    EXPECT_EQ(Nilai::Deserialize<int32_t>(V {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88}),
              0x11223344);
    EXPECT_EQ(
      Nilai::Deserialize<uint64_t>(V {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA}),
      0x1122334455667788);
    EXPECT_EQ(
      Nilai::Deserialize<int64_t>(V {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA}),
      0x1122334455667788);
}

TEST(NilaiDeserializer, DeserializeContainer)
{
    struct Foo
    {
        Foo(const std::vector<uint8_t>& v)
        {
            assert(v.size() == sizeof(uint16_t) + sizeof(float));
            auto it = v.begin();
            u16 = Nilai::Deserialize<uint16_t>(std::vector<uint8_t> {it, it + sizeof(uint16_t)});
            it += sizeof(uint16_t);
            f   = Nilai::Deserialize<float>(std::vector<uint8_t> {it, it + sizeof(float)});
        }

        uint16_t u16;
        float    f;
    };

    Foo f = Nilai::Deserialize<Foo>(std::vector<uint8_t>{0x11, 0x22, 0x3f, 0x9d, 0xf3, 0xb6});
    EXPECT_EQ(f.u16, 0x1122);
    EXPECT_EQ(f.f, 1.234f);
}
