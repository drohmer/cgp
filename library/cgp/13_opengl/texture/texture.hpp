#pragma once

#include "cgp/opengl_include.hpp"
#include "../debug/debug.hpp"

#include "cgp/04_grid_container/grid_container.hpp"
#include "cgp/07_image/image.hpp"




namespace cgp
{
	struct opengl_texture_image_structure
	{
		GLuint id = 0; // OpenGL ID for the texture

		int width;  // image width
		int height; // image height

		GLint format; // GL_RGB8, GL_RGBA8, GL_RGBF32

		GLenum texture_type; // = GL_TEXTURE_2D or GL_TEXTURE_CUBE_MAP

		void bind() const;
		void unbind() const;
		void clear();

		// Change the wrap of the texture to either
		//   GL_CLAMP_TO_BORDER, GL_CLAMP_TO_EDGE, GL_REPEAT, GL_MIRRORED_REPEAT 
		void update_wrap(GLint wrap_s, GLint wrap_t) const;


		// Shortcut to initialize a GL_TEXTURE_2D from an image described by its filename
		//  Similar to: initialize_texture_2d_on_gpu( image_load_file(filename), ...)
		void load_and_initialize_texture_2d_on_gpu(std::string const& filename, GLint wrap_s = GL_CLAMP_TO_EDGE, GLint wrap_t = GL_CLAMP_TO_EDGE, bool is_mipmap = true, GLint texture_mag_filter = GL_LINEAR, GLint texture_min_filter = GL_LINEAR_MIPMAP_LINEAR);



		// Initialize a GL_TEXTURE_2D from an image
		void initialize_texture_2d_on_gpu(image_structure const& im, GLint wrap_s = GL_CLAMP_TO_EDGE, GLint wrap_t = GL_CLAMP_TO_EDGE, bool is_mipmap = true, GLint texture_mag_filter = GL_LINEAR, GLint texture_min_filter = GL_LINEAR_MIPMAP_LINEAR);

		// Initialize a GL_TEXTURE_2D from a float grid
		void initialize_texture_2d_on_gpu(grid_2D<vec3> const& im, GLint wrap_s = GL_CLAMP_TO_EDGE, GLint wrap_t = GL_CLAMP_TO_EDGE, bool is_mipmap = true, GLint texture_mag_filter = GL_LINEAR, GLint texture_min_filter = GL_LINEAR_MIPMAP_LINEAR);

		// Initialize a CUBEMAP on GPU from 6 squared images
		void initialize_cubemap_on_gpu(image_structure const& x_neg, image_structure const& x_pos, image_structure const& y_neg, image_structure const& y_pos, image_structure const& z_neg, image_structure const& z_pos);

		// Initialize a generic GL_TEXTURE from empty data
		void initialize_texture_2d_on_gpu(int width_arg, int height_arg, GLint format_arg=GL_RGB8, GLenum texture_type_arg= GL_TEXTURE_2D, GLint wrap_s= GL_CLAMP_TO_EDGE, GLint wrap_t= GL_CLAMP_TO_EDGE, GLint texture_mag_filter= GL_LINEAR, GLint texture_min_filter= GL_LINEAR);

		// Update a 2D texture
		void update(grid_2D<vec3> const& im);
		void update(image_structure const& im);
	};

	// Read an image from file and initialize an opengl texture image from it
	GLuint opengl_load_texture_image(std::string const& filename, GLint wrap_s = GL_CLAMP_TO_EDGE, GLint wrap_t = GL_CLAMP_TO_EDGE);

	GLuint opengl_load_texture_image(image_structure const& im, GLint wrap_s=GL_CLAMP_TO_EDGE, GLint wrap_t=GL_CLAMP_TO_EDGE);
	GLuint opengl_load_texture_image(grid_2D<vec3> const& im, GLint wrap_s=GL_CLAMP_TO_EDGE, GLint wrap_t=GL_CLAMP_TO_EDGE);
	void opengl_update_texture_image(GLuint texture_id, grid_2D<vec3> const& im);
    
	
}