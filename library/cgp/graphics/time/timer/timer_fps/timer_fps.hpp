#pragma once

#include "../timer_event_periodic/timer_event_periodic.hpp"

namespace cgp
{

	class timer_fps
		: public timer_event_periodic
	{
	public:

		timer_fps(float update_fps_period=3.0f);
		float update();

		int fps;
	private:
		int counter;
	};

}