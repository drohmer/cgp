#include "cgp/01_base/base.hpp"
#include "camera_controller_first_person_euler.hpp"

namespace cgp
{
	void camera_controller_first_person_euler::update(mat4& camera_matrix_view) {
		camera_matrix_view = camera_model.matrix_view();
	}

	void camera_controller_first_person_euler::action_mouse_move(mat4& camera_matrix_view)
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


		if (event_valid) {
			if (click_left || (is_cursor_trapped && !click_right)) {
				if (!ctrl)
					camera_model.manipulator_rotate_roll_pitch_yaw(0, dp.y, -dp.x); // left drag => rotates
				else
					camera_model.manipulator_twist_rotation_axis(dp.x); // left drag + Ctrl => twist

			}
			else if (click_right)
				camera_model.manipulator_translate_front((p1 - p0).y); // right draw => move front/back
		}

		update(camera_matrix_view);
	}


	void camera_controller_first_person_euler::action_keyboard(mat4& )
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


	void camera_controller_first_person_euler::idle_frame(mat4& camera_matrix_view)
	{
		// Preconditions
		assert_cgp_no_msg(inputs != nullptr);
		assert_cgp_no_msg(window != nullptr);
		if (!is_active) return;

		float const magnitude = 2*inputs->time_interval;

		// displacement with WSAD
		if (inputs->keyboard.is_pressed(GLFW_KEY_R))
			camera_model.manipulator_translate_in_plane({ 0, magnitude }); // up
		if (inputs->keyboard.is_pressed(GLFW_KEY_F))
			camera_model.manipulator_translate_in_plane({ 0,-magnitude }); // down
		if (inputs->keyboard.is_pressed(GLFW_KEY_A))
			camera_model.manipulator_translate_in_plane({-magnitude ,0 }); // left
		if (inputs->keyboard.is_pressed(GLFW_KEY_D))
			camera_model.manipulator_translate_in_plane({magnitude ,0 }); // right
		if (inputs->keyboard.is_pressed(GLFW_KEY_W))
			camera_model.manipulator_translate_front(magnitude); // forward
		if (inputs->keyboard.is_pressed(GLFW_KEY_S))
			camera_model.manipulator_translate_front(-magnitude); // backward
		if(inputs->keyboard.is_pressed(GLFW_KEY_Q))
			camera_model.manipulator_twist_rotation_axis( magnitude); // twist left
		if (inputs->keyboard.is_pressed(GLFW_KEY_E))
			camera_model.manipulator_twist_rotation_axis(-magnitude); // twist right

		// with arrows:
		if (inputs->keyboard.ctrl == false) {
			if (inputs->keyboard.up)
				camera_model.manipulator_translate_front(magnitude); // forward
			if (inputs->keyboard.down)
				camera_model.manipulator_translate_front(-magnitude); // backward
		}
		else {
			if (inputs->keyboard.up)
				camera_model.manipulator_translate_in_plane({ 0,magnitude }); // up
			if (inputs->keyboard.down)
				camera_model.manipulator_translate_in_plane({ 0,-magnitude }); // down
		}
		if (inputs->keyboard.left)
			camera_model.manipulator_translate_in_plane({-magnitude ,0 }); // left
		if (inputs->keyboard.right)
			camera_model.manipulator_translate_in_plane({magnitude ,0 }); // right

		update(camera_matrix_view);

	}

	void camera_controller_first_person_euler::set_rotation_axis(vec3 const& rotation_axis)
	{
		camera_model.set_rotation_axis(rotation_axis);
	}
	void camera_controller_first_person_euler::set_rotation_axis_x()
	{
		camera_model.set_rotation_axis({ 1,0,0 });
	}
	void camera_controller_first_person_euler::set_rotation_axis_y()
	{
		camera_model.set_rotation_axis({ 0,1,0 });
	}
	void camera_controller_first_person_euler::set_rotation_axis_z()
	{
		camera_model.set_rotation_axis({ 0,0,1 });
	}

	void camera_controller_first_person_euler::look_at(vec3 const& eye, vec3 const& center, vec3 const& )
	{
		camera_model.look_at(eye, center);
	}

	std::string camera_controller_first_person_euler::doc_usage() const
	{
		std::string doc;
		doc += "First Person Euler - Camera that rotates around its own position using Euler angle (XYZ/rool-pitch-yaw Trait-Bryan convention).\n";
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