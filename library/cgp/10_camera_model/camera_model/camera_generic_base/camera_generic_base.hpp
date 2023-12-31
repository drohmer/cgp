#pragma once

#include "cgp/09_geometric_transformation/geometric_transformation.hpp"


namespace cgp
{
	struct camera_generic_base
	{
		virtual vec3 position() const = 0;
		virtual rotation_transform orientation() const = 0;

		frame to_frame() const;
		mat4 matrix_view() const;
		mat4 matrix_frame() const;

		vec3 front() const;
		vec3 up() const;
		vec3 right() const;
	};

	frame camera_frame_look_at(vec3 const& eye, vec3 const& center, vec3 const& up);

	vec3 camera_ray_direction(mat4 const& camera_frame, mat4 const& perspective_inverse, vec2 const& screen_position);
}