#include "cgp/01_base/base.hpp"
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
		vec2 const angles = rotation_speed * dp;

		bool const event_valid = !inputs->mouse.on_gui;
		bool const click_left = inputs->mouse.click.left;
		bool const click_right = inputs->mouse.click.right;
		bool const ctrl = inputs->keyboard.ctrl;


		if (event_valid) {
			if (click_left || (is_cursor_trapped && !click_right) ) {
				if(!ctrl)
					camera_model.manipulator_rotate_roll_pitch_yaw(0, -angles.y, angles.x); // left drag => rotates
				else
					camera_model.manipulator_rotate_roll_pitch_yaw(-angles.x, 0, 0); // left drag + Ctrl => twist

			}
			else if (click_right)
				camera_model.manipulator_translate_front(translation_speed * (p1 - p0).y); // right draw => move front/back

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

		float const magnitude = 2 * translation_speed * inputs->time_interval;
		float const angle_magnitude = 2 * rotation_speed * inputs->time_interval;


		if (inputs->keyboard.up || inputs->keyboard.is_pressed(GLFW_KEY_W))
			camera_model.manipulator_translate_front(magnitude);           // move front
		if (inputs->keyboard.down || inputs->keyboard.is_pressed(GLFW_KEY_S))
			camera_model.manipulator_translate_front(-magnitude);          // move back
		
		if (inputs->keyboard.left || inputs->keyboard.is_pressed(GLFW_KEY_A))
			camera_model.manipulator_translate_in_plane({ magnitude ,0 }); // move left
		if (inputs->keyboard.right || inputs->keyboard.is_pressed(GLFW_KEY_D))
			camera_model.manipulator_translate_in_plane({ -magnitude ,0 }); // move right

		if (inputs->keyboard.is_pressed(GLFW_KEY_R))
			camera_model.manipulator_translate_in_plane({ 0 , -magnitude }); // up
		if (inputs->keyboard.is_pressed(GLFW_KEY_F))
			camera_model.manipulator_translate_in_plane({ 0 , magnitude }); // down

		if (inputs->keyboard.is_pressed(GLFW_KEY_Q))
			camera_model.manipulator_rotate_roll_pitch_yaw(angle_magnitude, 0, 0);  // twist left
		if (inputs->keyboard.is_pressed(GLFW_KEY_E))
			camera_model.manipulator_rotate_roll_pitch_yaw(-angle_magnitude, 0, 0);  // twist right

		camera_matrix_view = camera_model.matrix_view();
	}

	void camera_controller_first_person::look_at(vec3 const& eye, vec3 const& center, vec3 const& up)
	{
		camera_model.look_at(eye, center, up);
	}

	void camera_controller_first_person::update(mat4& camera_matrix_view)
	{
		camera_matrix_view = camera_model.matrix_view();
	}

	std::string camera_controller_first_person::doc_usage() const
	{
		std::string doc;
		doc += "First Person - Camera that rotates around its own position.\n";
		doc += "Control: \n";
		doc += "   - Mouse left click + drag: Rotate the camera in its local left/right and up/down direction.\n";
		doc += "   - Mouse right click + drag: Translate the camera foward/backward.\n";
		doc += "   - Key arrows or WASD/ZQSD for translating forward/backward/left/right.\n";
		doc += "   - A(/Q)/E for twisting the camera left/right.\n";
		doc += "   - R/F for translating the camera up/down.\n";
		doc += "   - Press \"Shift+C\" (Maj C)  to enter Mouse-Captured Mode: Enables infinite mouse tracking (quit the mode with \"Shift+C\" again or Esc).";

		return doc;
	}
}