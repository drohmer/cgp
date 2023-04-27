#include "camera_controller_2d_displacement.hpp"

namespace cgp 
{
	void camera_controller_2d_displacement::action_mouse_move(mat4& camera_matrix_view)
	{
		// Preconditions
		assert_cgp_no_msg(inputs != nullptr);
		assert_cgp_no_msg(window != nullptr);
		if (!is_active) return;

		vec2 const& p1 = inputs->mouse.position.current;
		vec2 const& p0 = inputs->mouse.position.previous;
		vec2 const dp = p1 - p0;

		bool const event_valid = !inputs->mouse.on_gui;
		bool const click_left = inputs->mouse.click.left;
		bool const click_right = inputs->mouse.click.right;
		bool const ctrl = inputs->keyboard.ctrl;

		vec3 const& up = camera_model.axis_of_rotation;
		vec3 const& front = camera_model.front();
		vec3 const move_front = front - dot(front, up) * up;


		if (event_valid) {
			if (click_left || (is_cursor_trapped && !click_right)) {
				if (!ctrl)
					camera_model.manipulator_rotate_roll_pitch_yaw(0, dp.y, -dp.x); // left drag => rotates
				else
					camera_model.manipulator_twist_rotation_axis(dp.x); // left drag + Ctrl => twist

			}
			else if (click_right)
				camera_model.position_camera += (p1 - p0).y * move_front;
		}

		update(camera_matrix_view);
	}

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

	std::string camera_controller_2d_displacement::doc_usage() const
	{
		std::string doc;
		doc += "First person 2D Displacement Controller (constrained to move on a plane).\n";
		doc += "Control: \n";
		doc += "   - Mouse left click + drag: Rotate the camera in its local left/right and up/down direction.\n";
		doc += "   - Mouse right click + drag: Translate the camera foward/backward.\n";
		doc += "   - Key arrows or WASD/ZQSD for translating forward/backward/left/right.\n";
		doc += "   - Shift + Key: Moves faster.\n";
		doc += "   - Press \"Shift+C\" (Maj C)  to enter Mouse-Captured Mode: Enables infinite mouse tracking (quit the mode with \"Shift+C\" again or Esc).";

		return doc;
	}
}