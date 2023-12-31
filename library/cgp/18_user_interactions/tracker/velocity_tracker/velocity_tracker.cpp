#include "cgp/01_base/base.hpp"
#include "velocity_tracker.hpp"

namespace cgp
{
	void velocity_tracker::set_record(vec3 const& position, float time)
	{
		position_record = position;
		time_record = time;
	}
	void velocity_tracker::add(vec3 const& position, float time)
	{
		if(time-time_record<0.6f && time-time_record>0.01f){
			vec3 const dx = position-position_record;
			float const dt = time-time_record;
			vec3 const v = dx/dt;
			velocity = smoothing_weight * velocity + (1-smoothing_weight) * v;
		}
		set_record(position, time);
	}
}