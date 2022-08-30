#pragma once

#include <GLFW/glfw3.h>

#include "cgp/graphics/window/window.hpp"
#include "cgp/graphics/input_devices/input_devices.hpp"
#include "cgp/graphics/camera/camera_model/camera_model.hpp"

namespace cgp
{
	// A generic (empty) camera controller.
	//   The camera controller is set to provide a mapping between mouse/keyboard action onto the camera of the scene
	//   This mapping is set with the action functions:
	//     - action_mouse_move - called when the mouse is moved
	//     - action_keyboard - called when a keyboard touch is pressed/released
	//     - action_mouse_click - called when a mouse button is pressed/released
	//     - idle_frame - called at any frame (ex. for camera animation)
	//   This class and these functions can be specialized in derived controller class for specicif behavior.
	//
	//  Technical details:
	//    The camera controller has access to the global state of the user Inputs (keyboard/mouse), and to the Window via pointers.
	//    The controller doesn't own the camera class, but dedicated camera classes can be passed to the action functions
	struct camera_controller_generic_base
	{
		// Allow to activate/deactivate the camera
		bool is_active = true;

		// Assign the pointers of inputs and window to the global state
		// This method must be called at the initialization before any other action handling
		void initialize(input_devices& inputs, window_structure& window);


		void action_mouse_move(mat4&) {};
		void action_keyboard(mat4&) {};
		void action_mouse_click(mat4&) {};
		void idle_frame(mat4&) {};


		// Pointers to the global state of the inputs (keyboard, mouse, etc)
		input_devices* inputs = nullptr;
		// Pointer to the global state of the window
		window_structure* window = nullptr;


	};





}