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
	vec3 background_color; // Used in the main program

	// The position/orientation of a camera that can rotates freely around a specific position
	mat4 camera_view;

	// A projection structure (perspective or orthogonal projection)
	mat4 camera_projection;

	// The position of a light
	vec3 light;

	// Additional uniforms that can be attached to the environment if needed (empty by default)
	uniform_generic_structure uniform_generic;


	// This function will be called in the draw() call of a drawable element.
	//  The function is expected to send the uniform variables to the shader (e.g. camera, light)
	void send_opengl_uniform(opengl_shader_structure const& shader, bool expected = true) const override;

	environment_structure();

};



// Global variables storing general information on your project
struct project {

	// Global variable storing the relative path to the root of the project (access to shaders/, assets/, etc)
	//  Accessible via project::path
	static std::string path;

	// ImGui Window Scale: change this value (default=1) for larger/smaller gui window
	//  Accessible via project::gui_scale
	static float gui_scale;

	static bool fps_limiting;
	static float fps_max;
	static bool vsync;

};
