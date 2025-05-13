#pragma once


#include "cgp/cgp.hpp"
#include "environment.hpp"
#include "t5_devices.hpp"

using cgp::mesh_drawable;


struct gui_parameters {
	bool display_frame = true;
	bool display_wireframe = false;
};

// The structure of the custom scene
struct scene_structure : cgp::scene_inputs_generic {
	
	// ****************************** //
	// Elements and shapes of the scene
	// ****************************** //
	camera_controller_orbit_euler camera_control;
	camera_projection_perspective camera_projection;
	window_structure window;

	mesh_drawable global_frame;          // The standard global frame
	mesh_drawable quad;          		 // The tiltfive board
	environment_structure environment;   // Standard environment controler
	input_devices inputs;                // Storage for inputs status (mouse, keyboard, window dimension)
	gui_parameters gui;                  // Standard GUI element storage
	t5_devices t5;
	mesh_drawable grid;          		 // dsm

	// ****************************** //
	// Elements and shapes of the scene
	// ****************************** //



	// ****************************** //
	// Functions
	// ****************************** //

	void initialize();    // Standard initialization to be called before the animation loop
	void display_frame(); // The frame display to be called within the animation loop (all fbos)
	void display_frame_to_bound_fbo(); // The frame display to be called within the animation loop (to the currently bound fbo)
	void display_gui();   // The display of the GUI, also called within the animation loop


	void mouse_move_event();
	void mouse_click_event();
	void keyboard_event();
	void idle_frame();

};





