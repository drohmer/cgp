#include "input_devices.hpp"
#include "cgp/01_base/base.hpp"

#include <iostream>

namespace cgp {






void inputs_mouse_cursor_position_parameters::update(vec2 const& new_mouse_position) {
	previous = current;
	current = new_mouse_position;
}

void inputs_mouse_cursor_click_parameters::update_from_glfw_click(int button, int action)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		left = true;
		last_action = last_mouse_cursor_action::click_left;
	}
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		left = false;
		last_action = last_mouse_cursor_action::release_left;
	}

	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		right = true;
		last_action = last_mouse_cursor_action::click_right;
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
		right = false;
		last_action = last_mouse_cursor_action::release_right;
	}
}

void inputs_keyboard_parameters::update_from_glfw_key(int key, int action)
{
	last_action = { key, action };
	
	bool action_pressed = (action == GLFW_PRESS || action == GLFW_REPEAT);
	bool action_release = (action == GLFW_RELEASE);

	// Shift and CTRL
	if ((key == GLFW_KEY_LEFT_SHIFT || key == GLFW_KEY_RIGHT_SHIFT) && action_pressed)
		shift = true;
	if ((key == GLFW_KEY_LEFT_SHIFT || key == GLFW_KEY_RIGHT_SHIFT) && action_release)
		shift = false;

	if ((key == GLFW_KEY_LEFT_CONTROL || key == GLFW_KEY_RIGHT_CONTROL) && action_pressed)
		ctrl = true;
	if ((key == GLFW_KEY_LEFT_CONTROL || key == GLFW_KEY_RIGHT_CONTROL) && action_release)
		ctrl = false;

	// Up/Down/Left/Right
	if (key == GLFW_KEY_UP) {
		if (action_pressed) up = true;
		if (action_release) up = false;
	}

	if (key == GLFW_KEY_DOWN) {
		if (action_pressed) down = true;
		if (action_release) down = false;
	}

	if (key == GLFW_KEY_LEFT) {
		if (action_pressed) left = true;
		if (action_release) left = false;
	}

	if (key == GLFW_KEY_RIGHT) {
		if (action_pressed) right = true;
		if (action_release) right = false;
	}

	// General key
	if (action_pressed)
		pressed_keys_storage.insert(key);
	else
		pressed_keys_storage.erase(key);

	
}

inputs_keyboard_parameters::inputs_keyboard_parameters()
	:shift(false), ctrl(false), up(false), down(false), right(false), left(false)
{}

bool inputs_keyboard_last_action_parameter::is_pressed(int glfw_key) const
{
	if (action == GLFW_PRESS && glfw_key == key)
		return true;
	else
		return false;
}
bool inputs_keyboard_last_action_parameter::is_pressed(std::string const& keyname) const
{
	if (action != GLFW_PRESS)
		return false;

	const char* currentName = glfwGetKeyName(key, 0);
	if (currentName == nullptr) return false;

	if (keyname == currentName)
		return true;
	else
		return false;
}
bool inputs_keyboard_last_action_parameter::is_pressed(char keyname) const
{
	return is_pressed(str(keyname));
}

bool inputs_keyboard_last_action_parameter::is_released(int glfw_key) const
{
	if (action == GLFW_RELEASE && glfw_key == key)
		return true;
	else
		return false;
}
bool inputs_keyboard_last_action_parameter::is_released(std::string const& keyname) const
{
	if (action != GLFW_RELEASE)
		return false;

	const char* currentName = glfwGetKeyName(key, 0);
	if (currentName == nullptr) return false;

	if (keyname == currentName)
		return true;
	else
		return false;
}
bool inputs_keyboard_last_action_parameter::is_released(char keyname) const
{
	return is_released(str(keyname));
}


bool inputs_keyboard_parameters::is_pressed(int glfw_key) const
{
	if (pressed_keys_storage.find(glfw_key) != pressed_keys_storage.end())
		return true;
	return false;
}
bool inputs_keyboard_parameters::is_pressed(std::string const& keyname) const
{
	// Implementation detail: could improve the algorithm in storing a mapping between the string to the GLFW internal key description
	// See https://www.glfw.org/docs/3.3/group__keys.html
	for (auto key : pressed_keys_storage) {
		const char* currentName = glfwGetKeyName(key, 0);
		if (currentName != nullptr)
			if (keyname == currentName)
				return true;
	}

	return false;
}

bool inputs_keyboard_parameters::is_pressed(char keyname) const
{
	return is_pressed(str(keyname));
}







}