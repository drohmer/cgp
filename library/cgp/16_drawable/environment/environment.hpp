#pragma once

#include "cgp/13_opengl/shaders/shaders.hpp"

namespace cgp
{
	// An empty default environment used for standard drawable elements
	//  Environment are used as storage for uniform variables shared through all drawable elements.
	struct environment_generic_structure {

		// Default environment contains nothing
		//  Add the necessary variables in a derived class and send them using the functions opengl_uniform().

		static bool default_expected_uniform;

		// Override in the derived class the function send_opengl_uniform();
		virtual void send_opengl_uniform(opengl_shader_structure const& shader, bool expected = default_expected_uniform) const;
	};


}