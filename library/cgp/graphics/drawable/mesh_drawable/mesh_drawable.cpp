#include "mesh_drawable.hpp"

#include "cgp/core/base/base.hpp"

#if defined(__linux__) || defined(__EMSCRIPTEN__)
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

namespace cgp
{
	opengl_shader_structure mesh_drawable::default_shader;
	opengl_texture_image_structure mesh_drawable::default_texture;

	static void warning_initialize_non_empty();

	void mesh_drawable::initialize_data_on_gpu(mesh const& data, opengl_shader_structure const& shader_arg, opengl_texture_image_structure const& texture_arg)
	{
		// Error detection before sending the data to avoid unexpected behavior
		// *********************************************************************** //

		opengl_check;

		// Check if this mesh_drawable is already initialized
		if (vao != 0 || vbo_position.size != 0)
			warning_initialize_non_empty();

		if (data.position.size() == 0) {
			warning_cgp("Warning try to generate mesh_drawable with 0 vertex", "");
			return;
		}

		// Sanity check before sending mesh data to GPU
		assert_cgp(mesh_check(data), "Cannot send this mesh data to GPU in initializing mesh_drawable");


		// Variable initialization
		// *********************************************************************** //
		if(!(shader_arg.id==default_shader.id && shader.id!=0))
			shader = shader_arg;
		if(!(texture_arg.id==default_texture.id && texture.id!=0))
			texture = texture_arg;
		model = affine();
		material = material_mesh_drawable_phong();


		// Send the data to the GPU
		// ******************************************** //

		vbo_position.initialize_data_on_gpu(data.position);
		vbo_normal.initialize_data_on_gpu(data.normal);
		vbo_color.initialize_data_on_gpu(data.color);
		vbo_uv.initialize_data_on_gpu(data.uv);

		ebo_connectivity.initialize_data_on_gpu(data.connectivity);


		// Generate VAO
		glGenVertexArrays(1, &vao); opengl_check;
		glBindVertexArray(vao); opengl_check;
		opengl_set_vao_location(vbo_position, 0);
		opengl_set_vao_location(vbo_normal, 1);
		opengl_set_vao_location(vbo_color, 2);
		opengl_set_vao_location(vbo_uv, 3);
		glBindVertexArray(0); opengl_check;
	}

	void mesh_drawable::clear()
	{
		vbo_position.clear();
		vbo_normal.clear();
		vbo_color.clear();
		vbo_uv.clear();
		ebo_connectivity.clear();
		
		if(vao!=0)
			glDeleteVertexArrays(1, &vao);
		vao = 0;

		shader = opengl_shader_structure();
		model = affine();
		material = material_mesh_drawable_phong();
		texture = opengl_texture_image_structure();
		supplementary_texture.clear();

		opengl_check;
	}


	static void warning_initialize_non_empty()
	{
		std::string warning = "\n";
		warning += "  > You are calling initialize_gpu_data on an non-empty mesh_drawable \n";
		warning += "In normal condition, you should avoid initializing mesh_drawable on an existing one without clearing it - the previously allocated memory on the GPU is going to be lost.\n";
		warning += " - If you want to clear the memory, please call mesh_drawable.clear() before calling a new initialization\n";
		warning += " - Note that you should generally not call initialize_data_on_gpu() in the animation loop\n";

		warning_cgp("Calling initialize_data_on_gpu() on a mesh_drawable with non zero VBOs", warning);
	}


	void draw(mesh_drawable const& drawable, environment_generic_structure const& environment, uniform_generic_structure const& additional_uniforms, GLenum draw_mode)
	{
		opengl_check;
		// Initial clean check
		// ********************************** //
		// If there is not vertices or not triangles, returns
		//  (no error + does not display anything)
		if (drawable.vbo_position.size == 0 || drawable.ebo_connectivity.size == 0)
			return;

		assert_cgp(drawable.shader.id != 0, "Try to draw mesh_drawable without shader ");
		assert_cgp(!glIsShader(drawable.shader.id), "Try to draw mesh_drawable with incorrect shader ");
		assert_cgp(drawable.texture.id != 0, "Try to draw mesh_drawable without texture ");

		// Set the current shader
		// ********************************** //
		glUseProgram(drawable.shader.id); opengl_check;

		// Send uniforms for this shader
		// ********************************** //

		// send the uniform values for the model and material of the mesh_drawable
		drawable.send_opengl_uniform();

		// send the uniform values for the environment
		environment.send_opengl_uniform(drawable.shader);

		// [Optionnal] send any additional uniform for this specidic draw call
		additional_uniforms.send_opengl_uniform(drawable.shader);


		// Set textures
		// ********************************** //
		glActiveTexture(GL_TEXTURE0); opengl_check;
		drawable.texture.bind();
		opengl_uniform(drawable.shader, "image_texture", 0);  opengl_check;

		//Set any additional texture
		int texture_count = 1;
		for (auto const& element : drawable.supplementary_texture)
		{
			std::string const& additional_texture_name = element.first;
			opengl_texture_image_structure const& additional_texture = element.second;

			glActiveTexture(GL_TEXTURE0 + texture_count); opengl_check;
			additional_texture.bind();
			opengl_uniform(drawable.shader, additional_texture_name, texture_count);

			texture_count++;
		}


		// Prepare for draw call
		// ********************************** //
		glBindVertexArray(drawable.vao);                                     opengl_check;
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, drawable.ebo_connectivity.id); opengl_check;


		// Draw call
		// ********************************** //
		glDrawElements(draw_mode, GLsizei(drawable.ebo_connectivity.size * 3), GL_UNSIGNED_INT, nullptr); opengl_check;


		// Clean state
		// ********************************** //
		glBindVertexArray(0);
		drawable.texture.unbind();
		glUseProgram(0);
	}

	void draw_wireframe(mesh_drawable const& drawable, environment_generic_structure const& environment, vec3 const& color, uniform_generic_structure const& additional_uniforms)
	{
#ifndef __EMSCRIPTEN__ 		// Polygon Mode not available in WebGL
		mesh_drawable wireframe = drawable;
		wireframe.material.phong = { 1.0f,0.0f,0.0f,64.0f };
		wireframe.material.color = color;
		wireframe.material.texture_settings.active = false;
	
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glEnable(GL_POLYGON_OFFSET_LINE);
		glPolygonOffset(-1.0, 1.0);        opengl_check;
		draw(wireframe, environment, additional_uniforms);
		glDisable(GL_POLYGON_OFFSET_LINE); opengl_check;
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
#endif

	}


	void mesh_drawable::send_opengl_uniform(bool expected) const
	{
		// Final model matrix in the shader is: hierarchy_transform_model * model
		mat4 const model_shader = hierarchy_transform_model.matrix() * model.matrix();

		// The normal matrix is transpose( (hierarchy_transform_model * model)^{-1} )
		mat4 const model_normal_shader = transpose(inverse(model).matrix() * inverse(hierarchy_transform_model).matrix());
		

		// set the Model matrix
		opengl_uniform(shader, "model", model_shader, expected);
		opengl_uniform(shader, "modelNormal", model_normal_shader, expected);

		// set the material
		material.send_opengl_uniform(shader);
	}
}