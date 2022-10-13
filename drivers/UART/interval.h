#ifndef NILAI_DRIVERS_UART_INTERVAL_H_
#define NILAI_DRIVERS_UART_INTERVAL_H_
#include <cstdint>

namespace Nilai::Drivers::Uart
{
struct Interval
{
    Interval() = default;
    Interval(size_t s, size_t e) : start(s), end(e) {}

    size_t start = 0;
    size_t end   = 0;
};
}    // namespace Nilai::Drivers::Uart

#endif /* NILAI_DRIVERS_UART_INTERVAL_H_ */
