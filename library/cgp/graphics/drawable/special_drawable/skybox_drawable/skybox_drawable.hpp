#pragma once

#include "cgp/graphics/opengl/opengl.hpp"
#include "cgp/graphics/drawable/environment/environment.hpp"
#include "cgp/geometry/transform/affine/affine.hpp"
#include <string>

namespace cgp {



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
		affine model;


		void initialize_data_on_gpu();

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
	void draw(skybox_drawable const& drawable, environment_generic_structure const& environment = environment_generic_structure());

}


