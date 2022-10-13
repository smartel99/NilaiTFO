#ifndef NILAI_DRIVERS_UART_FRAME_H
#define NILAI_DRIVERS_UART_FRAME_H

#include <cstdint>
#include <string>
#include <vector>

namespace Nilai::Drivers::Uart
{

struct Frame
{
    std::vector<uint8_t> data;
    size_t               len       = 0;
    uint32_t             timestamp = 0;

    Frame() = default;


    Frame(const std::vector<uint8_t>& d, uint32_t t) : data(d), len(data.size()), timestamp(t) {}

    [[nodiscard]] std::string ToStr() const
    {
        return std::string {reinterpret_cast<const char*>(data.data())};
    }

    bool operator==(const std::string& s) const
    {
        return (std::string {reinterpret_cast<const char*>(data.data()), len} == s);
    }

    bool operator!=(const std::string& s) const { return !(*this == s); }
};

}    // namespace Nilai::Drivers::Uart
#endif
