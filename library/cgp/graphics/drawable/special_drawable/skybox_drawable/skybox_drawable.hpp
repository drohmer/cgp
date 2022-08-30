#pragma once

#include "cgp/graphics/opengl/opengl.hpp"
#include <string>

namespace cgp {



	//struct cubemap_image_structure {
	//	image_structure x_neg;
	//	image_structure x_pos;

	//	image_structure y_neg;
	//	image_structure y_pos;

	//	image_structure z_neg;
	//	image_structure z_pos;

	//	void load_from_single_image(std::string const& file_path);
	//};

	struct skybox_drawable {

		// Shader data
		opengl_shader_structure shader;

		// Texture image
		opengl_texture_image_structure texture;

		// Per-vertex data
		opengl_vbo_structure vbo_position;

		// Indexed connectivity
		opengl_ebo_structure ebo_connectivity;

		// VAO indicating the VBO organization
		GLuint vao;

		// Uniform
		mat4 model;


		void initialize_data_on_gpu();
		//void set_texture_images(image_structure const& x_neg, image_structure const& x_pos, image_structure const& y_neg, image_structure const& y_pos, image_structure const& z_neg, image_structure const& z_pos);
		//void load_texture_from_multiple_files(std::string const& directory_path);
		//void load_texture(cubemap_image_structure const& cubemap_images);

		void clear();

	};

	// Draw the Skybox
	//  Must be called before drawing the other shapes and without writing in the Depth Buffer
	//
	//  Expected syntax (In the drawing loop, no draw calls before) :
	//     glDepthMask(GL_FALSE); // disable depth-buffer writing
	//     draw(skybox, environment);
	//     glDepthMask(GL_TRUE);  // re-activate depth-buffer write
	//     ... other drawing calls comes after
	template <typename SCENE_ENVIRONMENT>
	void draw(skybox_drawable const& skybox, SCENE_ENVIRONMENT const& environment);
}

namespace cgp {

	template <typename SCENE_ENVIRONMENT>
	void draw(skybox_drawable const& skybox, SCENE_ENVIRONMENT const& environment)
	{
		if (skybox.shader.id == 0) {
			std::cout << "Attempt to draw a skybox that seems uninitialized (shaderID==0)" << std::endl;
			std::cout << "Call skybox.initialize_data_on_gpu() and load a texture before draw call" << std::endl;
			abort();
		}
		assert_cgp(skybox.vbo_position.size > 0, "Incorrect cube data");

		// Setup shader
		assert_cgp(skybox.texture.id != 0, "Try to draw skybox_drawable without texture ");
		glUseProgram(skybox.shader.id); opengl_check;


		// Send uniforms for this shader
		opengl_uniform(skybox.shader, "projection", environment.projection.matrix());
		opengl_uniform(skybox.shader, "view", environment.camera_view);
		opengl_uniform(skybox.shader, "model", skybox.model);

		// Set texture as a cubemap (different from the 2D texture using in the "standard" draw call)
		glActiveTexture(GL_TEXTURE0); opengl_check;
		skybox.texture.bind();
		opengl_uniform(skybox.shader, "image_skybox", 0);  opengl_check;

		// Call draw function
		glBindVertexArray(skybox.vao);   opengl_check;
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skybox.ebo_connectivity.id); opengl_check;
		glDrawElements(GL_TRIANGLES, GLsizei(skybox.ebo_connectivity.size * 3), GL_UNSIGNED_INT, nullptr); opengl_check;

		// Clean buffers
		glBindVertexArray(0);
		glActiveTexture(GL_TEXTURE0); opengl_check;
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

		glDepthMask(GL_TRUE);
	}
}

