/**
 * @file    profiler.cpp
 * @author  Serj Bashlayev
 *          https://github.com/Serj-Bashlayev
 *          email: phreak_ua@yahoo.com
 * @author  Samuel Martel
 * @date    2022-05-10
 * @brief Code time profiler with output to ITM Stimulus Port 0
 * Debug (printf) Viewer
 * Time accuracy 1uS
 * Example output:
 * Profiling "Start" sequence:
 *      --Event-----------------------|--timestamp--|---delta_t----
 *      GLCD_Init                     :       41 us | +       41 us
 *      OLED_Init                     :     5288 us | +     5247 us
 *      u8g_SetFont                   :     5292 us | +        4 us
 *      HAL_Delay(10)                 : 10004967 us | +  9999675 us
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
#include "profiler.h"

#if defined(NILAI_ENABLE_PROFILING)

#    include "../time.h"

#    include <cstdarg>
#    include <cstdio>
#    include <cstring>
#    include <string>

namespace Nilai
{

Profiler* Profiler::s_instance = nullptr;

Profiler::Profiler(const std::function<void(const char*, size_t)>& printStr, size_t reportFreq)
: m_reportFreq(reportFreq)
{
#    if !defined(NILAI_TEST)
    // These flags can only be set by a debugger.
    static constexpr uint32_t dbgFlags = SCB_DFSR_EXTERNAL_Msk | SCB_DFSR_VCATCH_Msk |
                                         SCB_DFSR_DWTTRAP_Msk | SCB_DFSR_BKPT_Msk |
                                         SCB_DFSR_HALTED_Msk;
    if (/*(ITM->TER & 1) == 1 &&*/ (SCB->DFSR & dbgFlags) != 0)
#    endif
    {
        // If ITM is enabled and one of the bits in the Debug Fault Status Register are set,
        // a debugger is connected.
        if (printStr)
        {
            m_printStrFunc = printStr;
            m_startFunc    = [this](const std::string_view& n) { StartImpl(n); };
            m_createFunc   = [this](const std::string_view& n) { return CreateImpl(n); };
            m_updateFunc   = [this](size_t id, uint32_t t) { UpdateImpl(id, t); };
            m_reportFunc   = [this]() { ReportImpl(); };

            m_events.reserve(NILAI_MAX_PROFILE_EVENTS);
        }
    }
}

void Profiler::Init(const std::function<void(const char*, size_t)>& printStr, size_t reportFreq)
{
    if (s_instance == nullptr)
    {
        s_instance = new Profiler(printStr, reportFreq);
    }
}

void Profiler::Deinit()
{
    delete s_instance;
    s_instance = nullptr;
}

void Profiler::StartImpl(const std::string_view& name)
{
    m_profName = name;
    m_events.clear();

#    if !defined(NILAI_TEST)
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;    // Enable counter.
#    endif
}

size_t Profiler::CreateImpl(const std::string_view& name)
{
    size_t id = INVALID_EVENT_ID;
    if (m_events.size() < NILAI_MAX_PROFILE_EVENTS)
    {
        // There's still room in the vector to add a new event.
        m_events.emplace_back(name);
        id = m_events.size() - 1;
    }
    return id;
}

void Profiler::UpdateImpl(size_t id, uint32_t wallTime)
{
    if (id < NILAI_MAX_PROFILE_EVENTS && id < m_events.size())
    {
        // If event exists, update it.
        ProfilingEvent& event = m_events[id];
        ++event.Hit;
        event.TotTime += static_cast<float>(wallTime);
        event.MinTime = std::min(event.MinTime, wallTime);
        event.MaxTime = std::max(event.MaxTime, wallTime);
    }
}

void Profiler::ReportImpl()
{
    if (GetTime() >= m_nextReportTime)
    {
        m_nextReportTime = GetTime() + m_reportFreq;
        Print(
          "\033[2J\033[1;1HProfiling \"%s\" sequence: \r\n"
          "--Event---------------------------|----Hit----|----Min----|----Max----|--Average--\r\n",
          m_profName.data());

        for (const auto& event : m_events)
        {
            Print("%-33s | %9d | %9d | %9d | %9d \n\r",
                  event.Name.data(),
                  event.Hit,
                  event.MinTime,
                  event.MaxTime,
                  static_cast<size_t>(event.TotTime / (float)event.Hit));
        }
    }
}

void Profiler::Print(const char* fmt, ...)
{
    static constexpr size_t MAX_SIZE = 1024;
    va_list                 args;
    va_start(args, fmt);

    static char buff[MAX_SIZE] = {};

    size_t s = vsnprintf(buff, MAX_SIZE, fmt, args);

    m_printStrFunc(buff, s);

    va_end(args);
}

#    if defined(NILAI_TEST)
ProfilingTimer::ProfilingTimer(size_t id) : m_id(id), m_startTime(GetTime())
{
}
ProfilingTimer::~ProfilingTimer()
{
    uint32_t end = GetTime();
    Profiler::UpdateEvent(m_id, end - m_startTime);
}
#    else
ProfilingTimer::ProfilingTimer(size_t id) : m_id(id), m_startTime(DWT->CYCCNT)
{
}
ProfilingTimer::~ProfilingTimer()
{
    uint32_t end = DWT->CYCCNT;
    Profiler::UpdateEvent(m_id, end - m_startTime);
}
#    endif
}    // namespace Nilai
#endif
