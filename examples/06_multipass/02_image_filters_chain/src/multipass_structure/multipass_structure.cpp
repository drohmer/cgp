#include "multipass_structure.hpp"

using namespace cgp;

void multipass_structure::initialize(
	std::string const& v_shader_1, std::string const& f_shader_1,
	std::string const& v_shader_2, std::string const& f_shader_2)
{
	// Initialize the FBO for the first pass
	fbo_1.initialize();
	fbo_2.initialize();

	// Initialize a simple quad used to display the result on the entire screen
	quad_1.initialize_data_on_gpu(mesh_primitive_quadrangle({ -1,-1,0 }, { 1,-1,0 }, { 1,1,0 }, { -1,1,0 }));
	quad_1.shader.load(v_shader_1, f_shader_1);

	quad_2.initialize_data_on_gpu(mesh_primitive_quadrangle({ -1,-1,0 }, { 1,-1,0 }, { 1,1,0 }, { -1,1,0 }));
	quad_2.shader.load(v_shader_2, f_shader_2);

}

void multipass_structure::update_screen_size(int width, int height)
{
	fbo_1.update_screen_size(width, height);
	fbo_2.update_screen_size(width, height);

	quad_1.texture = fbo_1.texture;
	quad_2.texture = fbo_2.texture;
}

void multipass_structure::start_pass_1()
{
	fbo_1.start();
}

void multipass_structure::end_pass_1()
{
	fbo_1.stop();
}

void multipass_structure::render_pass_2(environment_generic_structure const& environment)
{
	fbo_2.start();
	draw(quad_1, environment, false);
	fbo_2.stop();
}


void draw(multipass_structure const& element, environment_generic_structure const& environment)
{
	glDisable(GL_DEPTH_TEST);
	draw(element.quad_2, environment, false); 
	glEnable(GL_DEPTH_TEST);
}