#include "timer_basic.hpp"

#include <GLFW/glfw3.h>

namespace cgp
{

timer_basic::timer_basic()
	:t(0),scale(1.0f),running(true),time_previous(static_cast<float>(glfwGetTime()))
{}
float timer_basic::update()
{
	if(!running)
        return 0.0f;

    const float time_current = static_cast<float>(glfwGetTime());
    const float dt = scale*(time_current-time_previous);

    time_previous = time_current;
    t += dt;

    return dt;
}
void timer_basic::start()
{
    running = true;
    time_previous = static_cast<float>(glfwGetTime());
}
void timer_basic::stop()
{
    running = false;
}


}