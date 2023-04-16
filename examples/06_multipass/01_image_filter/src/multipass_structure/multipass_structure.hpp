#pragma once

#include "cgp/cgp.hpp"


struct multipass_structure {
	cgp::opengl_fbo_structure fbo;
	cgp::mesh_drawable quad_screen;

	void initialize(std::string const& vertex_shader_effect_path, std::string const& fragment_shader_effect_path);
	void update_screen_size(int width, int height);

	void start_pass_1();
	void end_pass_1();
};

void draw(multipass_structure const& element, cgp::environment_generic_structure const& environment);
