#pragma once

#include "../camera_controller_generic_base/camera_controller_generic_base.hpp"

namespace cgp
{
	// Specialized camera controller representing a "first person" view (i.e. always rotates around the current position of the camera)
	//  This camera model uses a quaternion to represent a rotation and implements by default a free-rotation mode in arbitrary direction.
	struct camera_controller_first_person : camera_controller_generic_base
	{
		camera_first_person camera_model;

		// Parameters to adjust camera speed
		float translation_speed = 1;
		float rotation_speed = 1;

		void action_mouse_move(mat4& camera_matrix_view);


		// Key 'C' (in capital) to capture the cursor
		void action_keyboard(mat4& camera_matrix_view);// camera_generic_base& camera);
				void idle_frame(mat4& camera_matrix_view);

		void update(mat4& camera_matrix_view);

		void look_at(vec3 const& eye, vec3 const& center, vec3 const& up);

		std::string doc_usage() const;

	protected:
		bool is_cursor_trapped = false; // true = cursor captured (/infinite motion), false = cursor free
	};
}