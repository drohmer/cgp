#pragma once

#include "cgp/geometry/vec/vec.hpp"

namespace cgp
{
	struct velocity_tracker
	{
		vec3 position_record;
		float time_record;

		vec3 velocity;
		float smoothing_weight = 0.6f;

		void set_record(vec3 const& position, float time);
		void add(vec3 const& position, float time);
	};
}