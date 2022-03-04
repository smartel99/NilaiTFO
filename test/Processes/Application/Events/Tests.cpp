/**
 * @file    Tests.cpp
 * @author  Samuel Martel
 * @date    2022-03-04
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

#include "../TestApp.h"
#include <gtest/gtest.h>

using namespace cep;

static int s_lastCall = -1;

static TestApp InitTest()
{
    TestApp app;

    for (size_t i = 0; i < NILAI_EVENTS_MAX_CALLBACKS; i++)
    {
        EXPECT_EQ(app.RegisterEventCallback(Events::EventTypes::Exti0,
                                            [](Events::Event*) { return false; }),
                  i);
    }

    return app;
}


TEST(Application_Events, Register)
{
    TestApp app;

    for (size_t i = 0; i < NILAI_EVENTS_MAX_CALLBACKS; i++)
    {
        EXPECT_EQ(app.RegisterEventCallback(Events::EventTypes::Exti0,
                                            [](Events::Event*) { return false; }),
                  i);
    }

    // Adding more callback than we have room for.
    EXPECT_EQ(
      app.RegisterEventCallback(Events::EventTypes::Exti0, [](Events::Event*) { return false; }),
      -1);

    // Adding callbacks of another type should still work too.
    EXPECT_EQ(
      app.RegisterEventCallback(Events::EventTypes::Exti1, [](Events::Event*) { return false; }),
      0);
}

TEST(Application_Events, Unregister)
{
    /**
     * @brief   Adds a single callback, then removes it, before adding another one.
     * The ID of the second one should be the same as the first one.
     *
     */
    TestApp app;

    size_t id =
      app.RegisterEventCallback(Events::EventTypes::Exti0, [](Events::Event*) { return false; });

    app.UnregisterEventCallback(Events::EventTypes::Exti0, id);

    EXPECT_EQ(
      app.RegisterEventCallback(Events::EventTypes::Exti0, [](Events::Event*) { return false; }),
      id);
}

TEST(Application_Events, TriggerSoftwareEvent)
{
    /**
     * @brief   Adds the maximum number of callbacks, then verifies that they get all called.
     */
    TestApp app;

    std::vector<bool> exti0Called(NILAI_EVENTS_MAX_CALLBACKS, false);
    std::vector<bool> exti1Called(NILAI_EVENTS_MAX_CALLBACKS, false);

    for (size_t i = 0; i < NILAI_EVENTS_MAX_CALLBACKS; i++)
    {
        app.RegisterEventCallback(Events::EventTypes::Exti0,
                                  [&exti0Called, i](Events::Event*)
                                  {
                                      exti0Called[i] = true;
                                      return false;
                                  });
        app.RegisterEventCallback(Events::EventTypes::Exti1,
                                  [&exti1Called, i](Events::Event*)
                                  {
                                      exti1Called[i] = true;
                                      return false;
                                  });
    }

    app.TriggerSoftEvent(Events::SoftwareEvents::Event0);

    // Only the EXTI0 callbacks should have been called.
    for (auto b : exti0Called)
    {
        EXPECT_TRUE(b);
    }
    for (auto b : exti1Called)
    {
        EXPECT_FALSE(b);
    }
}

TEST(Application_Events, TriggerDataEvent)
{
    /**
     * @brief   Adds the maximum number of callbacks, then verifies that they get all called.
     */
    TestApp app;

    std::vector<size_t> dataEvents(NILAI_EVENTS_MAX_CALLBACKS, 0);

    for (size_t i = 0; i < NILAI_EVENTS_MAX_CALLBACKS; i++)
    {
        app.RegisterEventCallback(Events::EventTypes::DataEvent,
                                  [&dataEvents, i](Events::Event* e)
                                  {
                                      Events::DataEvent* d = dynamic_cast<Events::DataEvent*>(e);
                                      dataEvents[i]        = d->As<size_t>();
                                      return false;
                                  });
    }

    static constexpr size_t val = 69;
    app.TriggerDataEvent(val);

    // The data should have been set.
    for (auto b : dataEvents)
    {
        EXPECT_EQ(b, val);
    }
}