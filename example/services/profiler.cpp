/**
 * @file    profiler.cpp
 * @author  Samuel Martel
 * @date    2022-08-08
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

#include "services/profiler/profiler.h"
#include "processes/application.h"
#include "services/time.h"

#include <iostream>

static void MyFunc()
{
    // Profile the execution time of the function.
    NILAI_PROFILE_FUNCTION();
    Nilai::Delay(1000);
}

int main()
{
    // Initializes the profiler with a function that will be used to output the profiling results.
    NILAI_PROFILING_INIT(
      [](const char* msg, [[maybe_unused]] size_t len) { std::cout << msg << std::endl; }, 1000);

    // Starts the profiling session.
    NILAI_PROFILING_START_SESSION("Profiler");

    Nilai::Application app;

    app.OnInit();
    if (app.OnPost())
    {
        // Profile the current scope.
        NILAI_PROFILE_SCOPE("Main run loop");
        // Nilai::Application::Run periodically prints the profiling results.
        app.Run();
    }

    // Terminates the profiling session.
    NILAI_PROFILING_DEINIT();
}
