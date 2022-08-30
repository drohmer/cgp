#pragma once

#include "third_party/src/imgui/imgui.h"
#include "third_party/src/imgui/imgui_impl_glfw.h"
#include "third_party/src/imgui/imgui_impl_opengl3.h"

#include <GLFW/glfw3.h>

namespace cgp
{
	void imgui_init(GLFWwindow* window);

	void imgui_create_frame();
	void imgui_render_frame(GLFWwindow* window);
	void imgui_cleanup();
}