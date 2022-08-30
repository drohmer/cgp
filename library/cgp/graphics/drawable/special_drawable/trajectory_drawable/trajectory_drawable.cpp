#include "trajectory_drawable.hpp"

namespace cgp
{
	trajectory_drawable::trajectory_drawable(size_t N_max_sample_arg)
		:position_record(), visual(), N_max_sample(N_max_sample_arg), current_size(0)
	{}

	void trajectory_drawable::clear()
	{
		position_record.clear();
		visual.clear();
		current_size = 0;
	}
	void trajectory_drawable::add(vec3 const& position)
	{

		// Initialize if needed
		if (position_record.size() == 0) {
			assert_cgp_no_msg(current_size == 0);
			assert_cgp_no_msg(visual.vbo_position.id == 0);

			position_record.resize(N_max_sample);
			visual.initialize_data_on_gpu(position_record);
		}
		assert_cgp_no_msg(position_record.size() == N_max_sample);

		for (size_t k = N_max_sample - 1; k > 0; --k) {
			position_record[k] = position_record[k - 1];
		}
		position_record[0] = position;

		if (current_size < N_max_sample)
			current_size++;

		visual.vbo_position.update(position_record);

	}

	void draw(trajectory_drawable const& drawable, environment_generic_structure const& environment)
	{
		if (drawable.current_size > 0) {
			draw(drawable.visual, environment);
		}
	}
}