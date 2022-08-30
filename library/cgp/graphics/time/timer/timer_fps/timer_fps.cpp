#include "timer_fps.hpp"

#include <GLFW/glfw3.h>

namespace cgp
{
    timer_fps::timer_fps(float update_fps_period)
        :timer_event_periodic(update_fps_period), fps(0), counter(0)
    {}

    float timer_fps::update()
    {
        float const elapsed_time = t_periodic;
        ++counter;

        float const dt = timer_event_periodic::update();
        if (event)
        {
            fps = counter/elapsed_time;
            counter = 0;
        }

        return dt;
    }



}