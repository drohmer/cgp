#include "cgp/01_base/base.hpp"
#include "timer_interval.hpp"

#include <GLFW/glfw3.h>

namespace cgp
{
	timer_interval::timer_interval(float t_min_arg, float t_max_arg)
        :timer_basic(), t_min(t_min_arg), t_max(t_max_arg)
    {
        assert_cgp_no_msg(t_max>t_min);
        timer_basic::t = t_min;
    }

    float timer_interval::update()
    {
        assert_cgp_no_msg(t_max>t_min);

        float dt = timer_basic::update();
        if(dt>t_max-t_min)
            dt = 0;

        if(t>=t_max)
            t = t - int((t-t_min)/(t_max-t_min)) * (t_max-t_min);
        if(t<t_min)
            t = t - int((t-t_min)/(t_max-t_min)-1) * (t_max-t_min);

        assert_cgp_no_msg(t>=t_min && t<t_max);
        return dt;
    }
    

}