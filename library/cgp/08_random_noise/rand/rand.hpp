#pragma once

namespace cgp
{

	/** Uniform random distribution defined on the interval [value_min, value_max]
	* default call rand_interval() generates uniform in [0,1]	*/
	float rand_uniform(float const value_min=0.0f, float const value_max=1.0f);

	/** Normal random distribution with specified averaged and stddev
	* default call rand_normal() is set with average=0, stddev=1*/
	float rand_normal(float const average = 0.0f, float const stddev = 1.0f);

	/** Reset the generator to an arbitrary number using the clock 
	 * Use this function if you want a different behavior of random at every new run */
	void rand_initialize_generator();

}
