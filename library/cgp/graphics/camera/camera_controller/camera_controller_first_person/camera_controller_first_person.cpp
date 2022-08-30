#include "cgp/core/base/base.hpp"
#include "camera_controller_first_person.hpp"

namespace cgp
{
	void camera_controller_first_person::action_mouse_move(mat4& camera_matrix_view)
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


		if (event_valid) {
			if (!is_cursor_trapped) {
				if (click_left)
					camera_model.manipulator_rotate_roll_pitch_yaw( 0, -dp.y, dp.x);
				else if (click_right)
					camera_model.manipulator_translate_front(-(p1 - p0).y);
			}
			else if (is_cursor_trapped)
				camera_model.manipulator_rotate_roll_pitch_yaw( 0, -dp.y, dp.x);
		}

		camera_matrix_view = camera_model.matrix_view();
	}




	void camera_controller_first_person::action_keyboard(mat4& )
	{
		if ( inputs->keyboard.last_action.is_pressed(GLFW_KEY_C) && inputs->keyboard.shift) {
			is_cursor_trapped = !is_cursor_trapped;
			if (is_cursor_trapped)
				glfwSetInputMode(window->glfw_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			else
				glfwSetInputMode(window->glfw_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		// Escape also gives back the cursor
		if (inputs->keyboard.last_action.is_pressed(GLFW_KEY_ESCAPE)) {
			is_cursor_trapped = false;
			glfwSetInputMode(window->glfw_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	}


	void camera_controller_first_person::idle_frame(mat4& camera_matrix_view)
	{
		// Preconditions
		assert_cgp_no_msg(inputs != nullptr);
		assert_cgp_no_msg(window != nullptr);
		if (!is_active) return;

		float const magnitude = 2*inputs->time_interval;
		float const angle_magnitude = 2*inputs->time_interval;


		// displacement with WSAD
		//   up/down
		if (inputs->keyboard.is_pressed(GLFW_KEY_R))
			camera_model.manipulator_translate_in_plane({ 0,-magnitude });
		if (inputs->keyboard.is_pressed(GLFW_KEY_F))
			camera_model.manipulator_translate_in_plane({ 0, magnitude });
		//   left/right
		if (inputs->keyboard.is_pressed(GLFW_KEY_A))
			camera_model.manipulator_translate_in_plane({ magnitude ,0 });
		if (inputs->keyboard.is_pressed(GLFW_KEY_D))
			camera_model.manipulator_translate_in_plane({ -magnitude ,0 });
		//   front/back
		if (inputs->keyboard.is_pressed(GLFW_KEY_W))
			camera_model.manipulator_translate_front(-magnitude);
		if (inputs->keyboard.is_pressed(GLFW_KEY_S))
			camera_model.manipulator_translate_front(magnitude);
		//   twist
		if (inputs->keyboard.is_pressed(GLFW_KEY_Q))
			camera_model.manipulator_rotate_roll_pitch_yaw(angle_magnitude, 0, 0);
		if (inputs->keyboard.is_pressed(GLFW_KEY_E))
			camera_model.manipulator_rotate_roll_pitch_yaw(-angle_magnitude, 0, 0);


		// With arrows
		if (inputs->keyboard.ctrl == false) {
			if (inputs->keyboard.up)
				camera_model.manipulator_translate_front(-magnitude);
			if (inputs->keyboard.down)
				camera_model.manipulator_translate_front(magnitude);
			if (inputs->keyboard.left)
				camera_model.manipulator_rotate_roll_pitch_yaw(angle_magnitude, 0, 0);
			if (inputs->keyboard.right)
				camera_model.manipulator_rotate_roll_pitch_yaw(-angle_magnitude, 0, 0);
		}
		else {
			if (inputs->keyboard.up)
				camera_model.manipulator_translate_in_plane({ 0,-magnitude });
			if (inputs->keyboard.down)
				camera_model.manipulator_translate_in_plane({ 0, magnitude });
			if (inputs->keyboard.left)
				camera_model.manipulator_translate_in_plane({ magnitude ,0 });
			if (inputs->keyboard.right)
				camera_model.manipulator_translate_in_plane({ -magnitude ,0 });
		}


		camera_matrix_view = camera_model.matrix_view();
	}
}