#include "emscripten.hpp"

#include <GLFW/glfw3.h>


namespace cgp
{
#ifndef __EMSCRIPTEN__
	void emscripten_update_window_size(int&, int&) {} // empty implementation in normal case
	void emscripten_set_glfw_window_hint() {} // empty implementation in normal case
#endif

#ifdef __EMSCRIPTEN__
	void emscripten_update_window_size(int& width, int& height)
	{
		width = emscripten_canvas_width();
		height = emscripten_canvas_height();
	}
	void emscripten_set_glfw_window_hint()
	{
		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	}

	int emscripten_canvas_width()
	{
		return static_cast<int>(EM_ASM_INT({ return document.getElementById('canvas').width; }));
	}
	int emscripten_canvas_height()
	{
		return static_cast<int>(EM_ASM_INT({ return document.getElementById('canvas').height; }));
	}
#endif
}

