#ifndef NILAI_DRIVERS_UART_FRAME_H
#define NILAI_DRIVERS_UART_FRAME_H

#include <cstdint>
#include <string>
#include <vector>

namespace Nilai::Drivers::Uart
{

struct Frame
{
    std::vector<uint8_t> Data;
    size_t               Len       = 0;
    uint32_t             Timestamp = 0;

    Frame() = default;


    Frame(std::vector<uint8_t> data, uint32_t timestamp)
    : Data(std::move(data)), Len(Data.size()), Timestamp(timestamp)
    {
    }

    [[nodiscard]] std::string ToStr() const
    {
        return std::string {reinterpret_cast<const char*>(Data.data())};
    }

    bool operator==(const std::string& s) const
    {
        return (std::string {reinterpret_cast<const char*>(Data.data()), Len} == s);
    }

    bool operator!=(const std::string& s) const { return !(*this == s); }
};

}    // namespace Nilai::Drivers::Uart
#endif
