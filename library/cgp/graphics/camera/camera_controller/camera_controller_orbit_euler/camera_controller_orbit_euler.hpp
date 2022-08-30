#pragma once

#include "../camera_controller_generic_base/camera_controller_generic_base.hpp"

namespace cgp
{

	struct camera_controller_orbit_euler : camera_controller_generic_base
	{
		camera_orbit_euler camera_model;

		void set_rotation_axis(vec3 const& rotation_axis);
		void set_rotation_axis_x();
		void set_rotation_axis_y();
		void set_rotation_axis_z();

		void look_at(vec3 const& eye, vec3 const& center, vec3 const& unused=vec3() /*Only for compatibility*/);

		void action_mouse_move(mat4& camera_matrix_view);
		void idle_frame(mat4& camera_matrix_view);

		void update(mat4& camera_matrix_view);
	};
}