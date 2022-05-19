/**
 * @file    profiler.h
 * @author  Samuel Martel
 * @date    2022-05-10
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
 * not, see <a href=https://www.gnu.org/licenses/>https://www.gnu.org/licenses/<a/>.
 */
#ifndef NILAI_PROFILER_H
#define NILAI_PROFILER_H

#if defined(NILAI_ENABLE_PROFILING)
#    include "../../defines/compiler_defines.h"
#    include "../../defines/internal_config.h"
#    include NILAI_HAL_HEADER

#    include <functional>
#    include <limits>
#    include <string_view>
#    include <vector>

#    define NILAI_PROFILING_INIT(func, freq)    ::Nilai::Profiler::Init(func, freq)
#    define NILAI_PROFILING_DEINIT()            ::Nilai::Profiler::Deinit();
#    define NILAI_PROFILING_START_SESSION(name) ::Nilai::Profiler::Start(name)

/**
 * @brief This macro breaks the one-statement rule. However, I can't find a way to circumvent it
 * without degrading ease of use.
 */
#    define NILAI_PROFILE_SCOPE(name)                                                              \
        static size_t nilaiProfilingTimerId##__LINE__ = ::Nilai::Profiler::CreateEvent(name);      \
        ::Nilai::ProfilingTimer nilaiProfilingTimer##__LINE__(nilaiProfilingTimerId##__LINE__)

#    define NILAI_PROFILE_FUNCTION() NILAI_PROFILE_SCOPE(FUNCSIG)

namespace Nilai
{
class Profiler
{
public:
    static constexpr size_t INVALID_EVENT_ID = std::numeric_limits<size_t>::max();

    /**
     * @brief Sets the function that will be used to output the profiling data.
     *
     * @example
     * <code>
     * // Data is sent on UART port 2.<br>
     * Nilai::Profiler::Init([](const char* fmt, size_t len)<br>
     *  {<br>
     *      HAL_UART_Transmit(&huart2, fmt, len, 50);<br>
     *  });
     * </code>
     * @param printStr A function that transmits a string.
     * @param reportFreq The frequency at which the profiling data should be reported.
     */
    static void Init(const std::function<void(const char*, size_t)>& printStr, size_t reportFreq);
    static void Deinit();

    /**
     * @brief Start profiler, save profiler name and start time
     *
     * @param name Profiler name
     */
    inline static void Start(const std::string_view& name) { s_instance->m_startFunc(name); }

    inline static size_t CreateEvent(const std::string_view& name)
    {
        return s_instance->m_createFunc(name);
    }

    inline static void UpdateEvent(size_t id, uint32_t wallTime)
    {
        s_instance->m_updateFunc(id, wallTime);
    }

    inline static void Report() { s_instance->m_reportFunc(); }

private:
    struct ProfilingEvent
    {
        std::string_view Name;                                      //! Name of the event.
        uint32_t MinTime = std::numeric_limits<uint32_t>::max();    //! Lowest execution time seen.
        uint32_t MaxTime = std::numeric_limits<uint32_t>::min();    //! Highest execution time seen.
        float    TotTime = 0;                                       //! Total execution time.
        uint32_t Hit     = 0;                                       //! Number of occurrences.

        ProfilingEvent(const std::string_view& name) : Name(name) {}
    };

private:
    Profiler(const std::function<void(const char*, size_t)>& printStr, size_t reportFreq);

    void   Print(const char* fmt, ...);
    void   StartImpl(const std::string_view& name);
    size_t CreateImpl(const std::string_view& name);
    void   UpdateImpl(size_t id, uint32_t wallTime);

    void ReportImpl();

private:
    static Profiler* s_instance;

    std::function<void(const char*, size_t)>       m_printStrFunc = [](const char*, size_t) {};
    std::function<void(const std::string_view&)>   m_startFunc    = [](const std::string_view&) {};
    std::function<size_t(const std::string_view&)> m_createFunc   = [](const std::string_view&)
    { return INVALID_EVENT_ID; };
    std::function<void(size_t, uint32_t)> m_updateFunc = [](size_t, uint32_t) {};
    std::function<void()>                 m_reportFunc = []() {};

    size_t m_reportFreq     = 0;
    size_t m_nextReportTime = 0;
    //! Name of the profiler.
    std::string_view m_profName;

    std::vector<ProfilingEvent> m_events = {};
};

class ProfilingTimer
{
public:
    ProfilingTimer(size_t id);
    ~ProfilingTimer();

private:
    size_t   m_id        = 0;
    uint32_t m_startTime = 0;
};

}    // namespace Nilai

#else
#    define NILAI_PROFILING_INIT(func, freq)
#    define NILAI_PROFILING_DEINIT()
#    define NILAI_PROFILING_START_SESSION(name)
#    define NILAI_PROFILE_SCOPE(name)
#    define NILAI_PROFILE_FUNCTION()
#endif

#endif    // NILAI_PROFILER_H
