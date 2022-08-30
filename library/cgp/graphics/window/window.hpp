#pragma once

#include <GLFW/glfw3.h>
#include "cgp/geometry/vec/vec2/vec2.hpp"
#include <string>

namespace cgp
{


	struct window_structure
	{
		GLFWwindow* glfw_window = nullptr;
		GLFWmonitor* monitor = nullptr;
		int x_pos=0, y_pos=0;
		int width=0, height=0;
		int screen_resolution_width=0, screen_resolution_height=0;
		bool is_full_screen = false;
		
		/** Generate a window using GLFW.
		* This function should be called at the beginning of the program before any OpenGL calls.
		* The function should only be called once.
		* The function initialize both GLFW and GLAD for OpenGL function access
		*/
		void initialize(int width = 0, int height = 0, std::string const& window_title = "cgp Display", int opengl_version_major = 3, int opengl_version_minor = 3);
		
		float aspect_ratio() const;

		void set_full_screen();
		void set_windowed_screen();

		// Convert an input coordinates expressed in pixels into a relative coordinates in screen normalized in [-1,1]
		vec2 convert_pixel_to_relative_coordinates(vec2 const& p_pixel) const;
	};


}