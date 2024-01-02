#include "environment.hpp"

std::string project::path = "";
float project::gui_scale = 1.0f;
bool project::fps_limiting = true;
float project::fps_max=60.0f;
bool project::vsync=true;

environment_structure::environment_structure()
{
	background_color = { 1,1,1 };
	light = { 1,1,1 };
}



void environment_structure::send_opengl_uniform(opengl_shader_structure const& shader, bool expected) const
{
	opengl_uniform(shader, "projection", camera_projection, expected);
	opengl_uniform(shader, "view", camera_view, expected);
	opengl_uniform(shader, "light", light, false);

	uniform_generic.send_opengl_uniform(shader, false);

}