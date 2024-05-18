#pragma once

#include "cgp/opengl_include.hpp"

#include "cgp/05_vec/vec.hpp"
#include "cgp/13_opengl/texture/texture.hpp"



namespace cgp {


	// Helper structure to handle a FBO - Frame Buffer Object
	//  Allows to store the rendering on the fbo instead of the color buffer for further use (texture, effect, etc)
	//  
	//  Usage:
	// 
	//  //Initialization stage
	//  | opengl_fbo_structure fbo;
	//  | fbo.initialize();
	//  | ...
	//  // Drawing pass
	//  | fbo.bind();
	//  | draw(myShape, environement);
	//  | fbo.unbind();
	// 
	//  The result image is stored in the texture variable

	enum class opengl_fbo_mode { image, depth };

	struct opengl_fbo_structure {

		// Mode of the FBO - image or depth
		//  image = stores the output of the rendering in a RGB texture
		//  depth = stores only depth of the rendering in a FLOAT texture
		opengl_fbo_mode mode = opengl_fbo_mode::image; 
		
		// ID of the FBO
		GLuint id; 

		// Depth buffer
		GLuint depth_buffer_id; 

		// Texture storing the rendering in this FBO
		opengl_texture_image_structure texture;

		// Size of the texture
		int width;
		int height;

		// Initialize the ids and the texture
		//  This function must be called before any rendering pass
		void initialize();

		// Start the rendering pass where the output will be stored on the FBO
		void bind() const;
		// Stop the rendering pass on the FBO
		void unbind() const;

		// Update the screen size (resize the texture if needed)
		void update_screen_size(int window_width, int windows_height);

	};
}