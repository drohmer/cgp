#pragma once

#include "../camera_controller_generic_base/camera_controller_generic_base.hpp"

namespace cgp
{
	struct camera_controller_first_person_euler : camera_controller_generic_base
	{
		camera_first_person_euler camera_model;

		void action_mouse_move(mat4& camera_matrix_view);

		// Key 'C' (in capital) to capture the cursor
		void action_keyboard(mat4& camera_matrix_view);

		// Keys up/down/left/right used to translate the camera
		//void idle_frame(camera_orbit_spherical_coord& camera);
		void idle_frame(mat4& camera_matrix_view);

		void update(mat4& camera_matrix_view);

	private:
		// true = cursor captured (/infinite motion), false = cursor free
		bool is_cursor_trapped = false; 
	};
}