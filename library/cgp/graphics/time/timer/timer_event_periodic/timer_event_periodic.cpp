#include "timer_event_periodic.hpp"

#include <GLFW/glfw3.h>

namespace cgp
{

    timer_event_periodic::timer_event_periodic(float event_period_arg)
        :timer_basic(), event(false), t_periodic(t), event_period(event_period_arg)
    {
    }

    float timer_event_periodic::update()
    {
        float dt = timer_basic::update();
        t_periodic += dt;
        if (t_periodic >= event_period)
        {
            t_periodic=0;
            event = true;
        }
        else 
            event = false;
        return dt;
    }




}