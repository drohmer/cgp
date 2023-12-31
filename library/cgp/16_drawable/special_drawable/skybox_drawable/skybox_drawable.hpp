#pragma once

#include "cgp/13_opengl/opengl.hpp"
#include "cgp/16_drawable/environment/environment.hpp"
#include "cgp/09_geometric_transformation/affine/affine.hpp"
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

		// Uniform model matrix
		affine model;

		// Unifom rotation of the skybox
		mat3 skybox_rotation;

		// Uniform alpha value for a color blending
		float alpha_color_blending = 0.0f;
		// Uniform color blending
		vec3 color_blending = vec3(1.0f, 1.0f, 1.0f);

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


