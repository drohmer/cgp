#include "multipass_structure.hpp"

using namespace cgp;

void multipass_structure::initialize()
{
	// Initialize the FBO for the first pass
	fbo_pass_1.initialize();

	// Initialize a simple quad used to display the result on the entire screen for 2nd pass
	quad_pass_2.initialize_data_on_gpu(mesh_primitive_quadrangle({ -1,-1,0 }, { 1,-1,0 }, { 1,1,0 }, { -1,1,0 }));

	// Set input texture of pass 2 <= Output texture output of pass1
	quad_pass_2.texture = fbo_pass_1.texture;
}

void multipass_structure::set_shader_pass_2(opengl_shader_structure const& shader)
{
	quad_pass_2.shader = shader;
}

void multipass_structure::update_screen_size(int width, int height)
{
	fbo_pass_1.update_screen_size(width, height);
	quad_pass_2.texture = fbo_pass_1.texture; // Optional: update texture size in texture structure if needed
}

void multipass_structure::clear_screen(vec3 const background_color) const
{
	glViewport(0, 0, fbo_pass_1.width, fbo_pass_1.height);
	glClearColor(background_color.x, background_color.y, background_color.z, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
}

void multipass_structure::start_pass_1(vec3 const background_color)
{
	fbo_pass_1.bind();
	clear_screen(background_color);
}

void multipass_structure::end_pass_1()
{
	fbo_pass_1.unbind();
}

void multipass_structure::start_pass_2(vec3 const background_color) {
	clear_screen(background_color);
}

void multipass_structure::draw_pass_2(environment_generic_structure const& environment) 
{
	glDisable(GL_DEPTH_TEST);
	draw(quad_pass_2, environment, false);
	glEnable(GL_DEPTH_TEST);
}

void multipass_structure::end_pass_2() 
{
	
}