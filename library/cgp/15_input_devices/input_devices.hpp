#pragma once

#include <GLFW/glfw3.h>
#include "cgp/05_vec/vec.hpp"
#include <set>


namespace cgp {


	// Position of the mouse stored in relative coordinates on screen
struct inputs_mouse_cursor_position_parameters {
	vec2 current;
	vec2 previous;

	// Update to a new position
	void update(vec2 const& new_mouse_position);
};

enum class last_mouse_cursor_action { click_left, click_right, release_left, release_right };
struct inputs_mouse_cursor_click_parameters {
	bool left  = false;
	bool right = false;
	last_mouse_cursor_action last_action;
	void update_from_glfw_click(int button, int action);
};


struct inputs_mouse_parameters {
	inputs_mouse_cursor_position_parameters position; // Store current and previous position of the mouse cursor in OpenGL relative coordinates [-1,1]
	inputs_mouse_cursor_click_parameters click;       // Information on clicked button (left/right)
	float scroll= 0;                                  // Store the amount of scroll
	bool on_gui = false;                              // true if the cursor currently on ImGui GUI
};

struct inputs_keyboard_last_action_parameter {
	int key;     // GLFW_KEY_...
	int action;  // GLFW_KEY_PRESS or GLFW_RELEASE

	bool is_pressed(int glfw_key) const;
	bool is_pressed(char keyname) const;
	bool is_pressed(std::string const& keyname) const;

	bool is_released(int glfw_key) const;
	bool is_released(char keyname) const;
	bool is_released(std::string const& keyname) const;
};



struct inputs_keyboard_parameters {

	// Short-hand to check if common keys are currently pressed
	//  The value of these key are true as long as they are not released
	//  Similar to is_pressed(GLFW_KEY_{UP/DOWN/SHIFT ...})
	bool shift; // handle both GLFW_KEY_LEFT_SHIFT and GLFW_KEY_RIGHT_SHIFT
	bool ctrl;  // handle both GLFW_KEY_LEFT_CONTROL and GLFW_KEY_RIGHT_CONTROL
	bool up, down, right, left;

	// Current status of keys (pressed or released)
	// Return if a key is currently pressed
	//  Return true as long as the key is not released
	bool is_pressed(int glfw_key) const;
	bool is_pressed(char keyname) const;
	bool is_pressed(std::string const& keyname) const;

	// Last key pressed or released
	inputs_keyboard_last_action_parameter last_action;


	inputs_keyboard_parameters();
	void update_from_glfw_key(int key, int action);

//private:
	std::set<int> pressed_keys_storage; // store all currently pressed key in their GLFW format (GLFW_KEY_{A/B/C/, etc})
};




struct input_devices {

	inputs_keyboard_parameters  keyboard;
	inputs_mouse_parameters     mouse;
	float time_interval = 0;

};



// Basic scene input receiver
// Does nothing by default - allows to 
struct scene_inputs_generic {
	void mouse_move_event() {};
	void mouse_click_event() {};
	void mouse_scroll_event() {};
	void keyboard_event() {};
	void idle_frame() {};
};


}