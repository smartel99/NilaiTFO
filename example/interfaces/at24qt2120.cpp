/**
 * @file    at24qt2120.cpp
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

#include "interfaces/AT24QT2120/at24qt2120.h"
#include "defines/module.h"

#include "processes/application.h"

class MyTouchSensor : public Nilai::Module
{
    using At24qt2120 = Nilai::Interfaces::At24Qt2120;

    using Keys         = Nilai::Interfaces::AT24QT2120::Keys;
    using Groups       = Nilai::Interfaces::AT24QT2120::Group;
    using SensorStatus = Nilai::Interfaces::AT24QT2120::SensorStatus;
    using Events       = Nilai::Events::EventTypes;

public:
    MyTouchSensor()
    {
        // clang-format off
        // Build and create the touch sensor using the I2C1 peripheral.
        m_touchSensor = At24qt2120::Create(&hi2c1)
                          .UseInInterruptMode(Events::Exti0)    // Pin is used in interrupt mode.
                          .SetKeyOptions(Keys::Key0)    // Set the options for key 0.
                            .InGroup(Groups::Group1)
                            .IsGuard(false)
                            .IsOutput(false)
                            .Complete()
                          .Build();
        // clang-format on
    }

    void Run() override
    {
        // Call the touch sensor's run function to refresh its state.
        m_touchSensor.Run();

        // If a new event occurred:
        if (m_touchSensor.GetLastEventTime() != m_lastEventTime)
        {
            // Go read it.
            m_lastEventTime     = m_touchSensor.GetLastEventTime();
            SensorStatus status = m_touchSensor.GetSensorStatus();

            LOG_INFO("Status: %08X", static_cast<uint16_t>(status.KeyStatuses));
        }
    }

private:
    At24qt2120 m_touchSensor;
    uint32_t   m_lastEventTime = 0;
};

int main()
{
    Nilai::Application app;

    app.OnInit();

    app.AddModule<MyTouchSensor>();

    if (app.OnPost())
    {
        app.Run();
    }
}
