#pragma once

#include "../timer_basic/timer_basic.hpp"

namespace cgp
{

	class timer_interval
		: public timer_basic
	{
	public:

		timer_interval(float t_min=0.0f, float t_max=1.0f);
		float update();

		float t_min;
		float t_max;
	};

}