#include "cgp/16_drawable/curve_drawable/curve_drawable.hpp"

namespace cgp
{
	struct trajectory_drawable
	{
		trajectory_drawable(size_t N_max_sample = 100);
		void clear();
		void add(vec3 const& position);


		numarray<vec3> position_record;
		curve_drawable visual;
		size_t N_max_sample;
		size_t current_size;

	};


	void draw(trajectory_drawable const& drawable, environment_generic_structure const& environment);
}