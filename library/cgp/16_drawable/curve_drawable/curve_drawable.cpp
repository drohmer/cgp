#include "cgp/01_base/base.hpp"
#include "curve_drawable.hpp"

namespace cgp
{
	opengl_shader_structure curve_drawable::default_shader;

	static void warning_initialize_non_empty()
	{
		std::string warning = "\n";
		warning += "  > You are calling initialize_gpu_data on an non-empty curve_drawable \n";
		warning += "In normal condition, you should avoid initializing curve_drawable on an existing one without clearing it - the previously allocated memory on the GPU is going to be lost.\n";
		warning += " - If you want to clear the memory, please call curve_drawable.clear() before calling a new initialization\n";
		warning += " - Note that you should generally not call initialize_data_on_gpu() in the animation loop\n";

		warning_cgp("Calling initialize_data_on_gpu() on a mesh_drawable with non zero VBOs", warning);
	}

	void curve_drawable::initialize_data_on_gpu(numarray<vec3> const& position, opengl_shader_structure const& shader_arg)
	{
		// Error detection before sending the data to avoid unexpected behavior
		// *********************************************************************** //
		opengl_check;

		if (vao != 0 || vbo_position.size != 0)
			warning_initialize_non_empty();

		if (position.size() == 0) {
			warning_cgp("Warning try to generate curve_drawable with 0 vertex", "");
			return;
		}

		// Variable initialization
		// *********************************************************************** //
		shader = shader_arg;
		model = affine();
		color = vec3(1, 0, 0);

		// Send the data to the GPU
		// ******************************************** //
		vbo_position.initialize_data_on_gpu(position);

		// Generate VAO
		glGenVertexArrays(1, &vao); opengl_check;
		glBindVertexArray(vao); opengl_check;
		opengl_set_vao_location(vbo_position, 0);
		glBindVertexArray(0);

	}


	void curve_drawable::clear()
	{
		vbo_position.clear(); opengl_check;

		glDeleteVertexArrays(1, &vao); opengl_check;
		vao = 0;
		shader.id = 0;
		model = affine();
		color = {0,0,0};
	}

	void curve_drawable::send_opengl_uniform(bool expected) const
	{
		opengl_uniform(shader, "color", color, expected);
		opengl_uniform(shader, "model", model.matrix(), expected);
	}


	void draw(curve_drawable const& drawable, environment_generic_structure const& environment, int N_points)
	{
		// Initial clean check
		// ********************************** //
		// If there is not vertices or not triangles, returns
		//  (no error + does not display anything)
		if (drawable.vbo_position.size == 0)
			return;

		// Set the current shader
		// ********************************** //
		assert_cgp(drawable.shader.id != 0, "Try to draw curve_drawable without shader");
		glUseProgram(drawable.shader.id); opengl_check;

		// Send uniforms for this shader
		// ********************************** //

		// Send internal variables as uniform (color, model)
		drawable.send_opengl_uniform();

		// special uniform sending the environment properties
		environment.send_opengl_uniform(drawable.shader);

		// Prepare for draw call
		// ********************************** //
		int const N_points_display = N_points < 0 ? drawable.vbo_position.size : N_points;
		glBindVertexArray(drawable.vao); opengl_check;
		if (drawable.display_type == curve_drawable_display_type::Curve) {
			glDrawArrays(GL_LINE_STRIP, 0, N_points_display); opengl_check;
		}
		else {
			glDrawArrays(GL_LINES, 0, N_points_display); opengl_check;
		}


		// Clean buffers
		glBindVertexArray(0);
		glUseProgram(0);
		opengl_check;
	}

}