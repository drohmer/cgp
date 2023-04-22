#include "camera_controller_2d_displacement.hpp"

namespace cgp 
{


	void camera_controller_2d_displacement::idle_frame(mat4& camera_matrix_view)
	{
		// Preconditions
		assert_cgp_no_msg(inputs != nullptr);
		assert_cgp_no_msg(window != nullptr);
		if (!is_active) return;

		float magnitude = 1.5f * inputs->time_interval;

		vec3 const& up = camera_model.axis_of_rotation;
		vec3 const& front = camera_model.front();
		vec3 const& right = camera_model.right();

		// run mode
		if (inputs->keyboard.shift)
			magnitude = 3.0f * magnitude;

		// Constraint displacement on the plane
		vec3 move_front = front - dot(front, up) * up;
		vec3 move_right = right - dot(right, up) * up;


		if (inputs->keyboard.up || inputs->keyboard.is_pressed(GLFW_KEY_W))
			camera_model.position_camera += magnitude * move_front;
		if (inputs->keyboard.down || inputs->keyboard.is_pressed(GLFW_KEY_S))
			camera_model.position_camera -= magnitude * move_front;
		if (inputs->keyboard.left || inputs->keyboard.is_pressed(GLFW_KEY_A))
			camera_model.position_camera -= 0.5f * magnitude * move_right;
		if (inputs->keyboard.right || inputs->keyboard.is_pressed(GLFW_KEY_D))
			camera_model.position_camera += 0.5f * magnitude * move_right;


		update(camera_matrix_view);
	}


}