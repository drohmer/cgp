#pragma once

#include "../curve_drawable/curve_drawable.hpp"

namespace cgp
{
	/** Extended curve_drawable that can handle dynamic addition of points using .push_back()
	* Reserve some extra space in the VBO, and resize it if needed when adding new points.*/
	struct curve_drawable_dynamic_extend : curve_drawable
	{
		curve_drawable_dynamic_extend();

		curve_drawable_dynamic_extend& clear();

		// Initialize the VBOs to store initial_capacity points
		curve_drawable_dynamic_extend& initialize_data_on_gpu(int initial_capacity = 100, opengl_shader_structure const& shader = curve_drawable::default_shader);

		// Add a new point in the VBO. 
		curve_drawable_dynamic_extend& push_back(cgp::vec3 const& p);

		// Store the number of valid points. When the actual number of points reach the capacity, the VBO is resized to double capacity.
		int N_valid_points;
	};

	void draw(curve_drawable_dynamic_extend const& drawable, environment_generic_structure const& environment);
}



