#pragma once

#include "../timer_basic/timer_basic.hpp"

namespace cgp
{

	class timer_event_periodic
		: public timer_basic
	{
	public:

		timer_event_periodic(float event_period=1.0f);
		float update();

		bool event;
		float t_periodic;
		float event_period;
	};

}