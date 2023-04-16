#include "multipass_structure.hpp"

using namespace cgp;

void multipass_structure::initialize(std::string const& vertex_shader_effect_path, std::string const& fragment_shader_effect_path)
{
	// Initialize the FBO for the first pass
	fbo.initialize();

	// Initialize a simple quad used to display the result on the entire screen
	quad_screen.initialize_data_on_gpu(mesh_primitive_quadrangle({ -1,-1,0 }, { 1,-1,0 }, { 1,1,0 }, { -1,1,0 }));
	quad_screen.shader.load(vertex_shader_effect_path, fragment_shader_effect_path); 

}

void multipass_structure::update_screen_size(int width, int height)
{
	fbo.update_screen_size(width, height);
	quad_screen.texture = fbo.texture; // update texture size (optionnal - only for coherence)
}

void multipass_structure::start_pass_1()
{
	fbo.start();
}

void multipass_structure::end_pass_1()
{
	fbo.stop();
}

void draw(multipass_structure const& element, environment_generic_structure const& environment)
{
	glDisable(GL_DEPTH_TEST);
	draw(element.quad_screen, environment, false); // display the quad with expected_uniform=false to avoid warning about non-used uniforms for standard rendering
	glEnable(GL_DEPTH_TEST);
}