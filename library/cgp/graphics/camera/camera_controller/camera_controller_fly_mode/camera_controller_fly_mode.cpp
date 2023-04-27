#include "camera_controller_fly_mode.hpp"

namespace cgp
{

void camera_controller_fly_mode::action_mouse_move(mat4& camera_matrix_view)
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

	if (event_valid) {
		if (!is_cursor_trapped) {
			if (click_left)
				camera_model.manipulator_rotate_roll_pitch_yaw(-dp.x, dp.y, 0);
		}
		else if (is_cursor_trapped)
			camera_model.manipulator_rotate_roll_pitch_yaw(-dp.x, dp.y, 0);
	}

	update(camera_matrix_view);
}


void camera_controller_fly_mode::idle_frame(mat4& camera_matrix_view)
{
	// Preconditions
	assert_cgp_no_msg(inputs != nullptr);
	assert_cgp_no_msg(window != nullptr);
	if (!is_active) return;

	float const magnitude = inputs->time_interval;


	if (inputs->keyboard.up || inputs->keyboard.is_pressed(GLFW_KEY_W))
		camera_model.manipulator_rotate_roll_pitch_yaw(0, pitch * magnitude, 0);
	if (inputs->keyboard.down || inputs->keyboard.is_pressed(GLFW_KEY_S))
		camera_model.manipulator_rotate_roll_pitch_yaw(0, -pitch * magnitude, 0);

	if (inputs->keyboard.left || inputs->keyboard.is_pressed(GLFW_KEY_A))
		camera_model.manipulator_rotate_roll_pitch_yaw(roll * magnitude, 0.0f, 0.0f);
	if (inputs->keyboard.right || inputs->keyboard.is_pressed(GLFW_KEY_D))
		camera_model.manipulator_rotate_roll_pitch_yaw(-roll * magnitude, 0.0, 0.0f);

	if (inputs->keyboard.is_pressed(GLFW_KEY_KP_ADD) || inputs->keyboard.is_pressed(GLFW_KEY_R))
		speed = std::min(speed * speed_increase, speed_max);
	if (inputs->keyboard.is_pressed(GLFW_KEY_KP_SUBTRACT) || inputs->keyboard.is_pressed(GLFW_KEY_F))
		speed = std::max(speed / speed_increase, speed_min);

	camera_model.manipulator_translate_front(speed * magnitude);

	update(camera_matrix_view);
}

std::string camera_controller_fly_mode::doc_usage() const
{
	std::string doc;
	doc += "First person 'fly mode': Continuously move forward, while being able to rotate (pull up/down and roll).\n";
	doc += "Control: \n";
	doc += "   - Mouse left click + drag: Pull up/down, roll right/left.\n";
	doc += "   - Key arrows or WASD/ZQSD for translating forward/backward/left/right.\n";
	doc += "   - Key +/- or R/F for speeding up/down.\n";
	doc += "   - Press \"Shift+C\" (Maj C)  to enter Mouse-Captured Mode: Enables infinite mouse tracking (quit the mode with \"Shift+C\" again or Esc).";

	return doc;
}

}