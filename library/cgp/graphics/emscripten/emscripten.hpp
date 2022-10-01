#pragma once

#include "cgp/opengl_include.hpp"



namespace cgp
{
	// Function used when emscripten is active (otherwise does nothing)
	//  Update width and height with the canvas size
	void emscripten_update_window_size(int& width, int& height);

	// Specific GLFW window hints for emscripten - OpenGL ES
	void emscripten_set_glfw_window_hint();

#ifdef __EMSCRIPTEN__
	int emscripten_canvas_width();
	int emscripten_canvas_height();
#endif
}

