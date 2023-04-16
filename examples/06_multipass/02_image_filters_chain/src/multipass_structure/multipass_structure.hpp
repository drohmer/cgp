#pragma once

#include "cgp/cgp.hpp"


struct multipass_structure {
	cgp::opengl_fbo_structure fbo_1;
	cgp::opengl_fbo_structure fbo_2;

	cgp::mesh_drawable quad_1;
	cgp::mesh_drawable quad_2;

	void initialize(
		std::string const& v_shader_1, std::string const& f_shader_1,
		std::string const& v_shader_2, std::string const& f_shader_2);

	void update_screen_size(int width, int height);

	void start_pass_1();
	void end_pass_1();

	void render_pass_2(cgp::environment_generic_structure const& environment);
};

void draw(multipass_structure const& element, cgp::environment_generic_structure const& environment);
