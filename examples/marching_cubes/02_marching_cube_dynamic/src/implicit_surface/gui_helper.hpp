#pragma once

#include "cgp/cgp.hpp"
#include "field_function.hpp"

// Element of the GUI that are not already stored in other structures
struct gui_parameters {

	struct { // Elements that are displayed
		bool frame = false;
		bool wireframe = false; // Display the wireframe of the implicit surface
		bool surface = true;    // Display the implicit surface
		bool domain = true;     // Display the box representing the domain of the marching cube
	} display;

	struct { // Elements of the domain
		int samples = 30;
		cgp::vec3 length = { 5,3,3 };
	} domain;

	// Isovalue used during the marching cube
	float isovalue = 0.5f;
};


void display_gui_implicit_surface(bool& is_update_field, bool& is_update_marching_cube, bool& is_save_obj, gui_parameters& gui, field_function_structure& field_function);