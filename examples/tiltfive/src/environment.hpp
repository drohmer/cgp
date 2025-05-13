#pragma once

#include "cgp/cgp.hpp"

using namespace cgp;

// ********************************************************************* //
// This file contains variables defining your global scene environment.
// ********************************************************************* //


// An environment structure contains variables that are needed to the drawing of an element in the scene, but that are not related to a particular shape or mesh.
// The environment contains typically the camera and the light.
struct environment_structure : environment_generic_structure
{
	// Color of the background of the scene
	vec3 background_color = {1,1,1}; // Used in the main program

	// The position/orientation of a camera that can rotates freely around a specific position
	mat4 camera_view;

	// A projection structure (perspective or orthogonal projection)
	mat4 camera_projection;

	// The position of a light
	vec3 light = {1,1,1};

	// Additional uniforms that can be attached to the environment if needed (empty by default)
	uniform_generic_structure uniform_generic;


	// This function will be called in the draw() call of a drawable element.
	//  The function is expected to send the uniform variables to the shader (e.g. camera, light)
	void send_opengl_uniform(opengl_shader_structure const& shader, bool expected = default_expected_uniform) const override;


};




// Global variables storing general information on your project
// Note: the default values are in the file environment.cpp
struct project {

	// Global variable storing the relative path to the root of the project (access to shaders/, assets/, etc)
	//  Accessible via project::path
	static std::string path;

	// ImGui Window Scale: change this value (default=1) for larger/smaller gui window
	static float gui_scale;

	// Window refresh rate
	static bool fps_limiting; // Is FPS limited automatically
	static float fps_max; // Maximal default FPS (used only of fps_max is true)
	static bool vsync; // Automatic synchronization of GLFW with the vertical-monitor refresh

	// Initial window size: expressed as ratio of screen in [0,1], or absolute pixel value if > 1
	static float initial_window_size_width;
	static float initial_window_size_height;

};
