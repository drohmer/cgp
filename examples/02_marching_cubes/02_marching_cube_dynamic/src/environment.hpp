#pragma once

#include "cgp/cgp.hpp"

using namespace cgp;


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

	environment_structure();
	void send_opengl_uniform(opengl_shader_structure const& shader, bool expected = true) const override;

};
