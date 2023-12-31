#pragma once

#include "cgp/opengl_include.hpp"

namespace cgp
{
	struct opengl_gpu_buffer_details {
		// The total size taken in memory of the entire buffer
		GLuint size_byte = 0;

		// How to read the content of the buffer
		GLuint size_element = 0; // The number of sub-element for 1 element (ex. 3 for a vec3, 2 for a vec2, etc)
		GLenum type_element = 0; // The type of each component of the buffer (ex. GL_FLOAT, GL_UNSIGNED_INT, etc)
		// Note: assume offset=0, and stride=0
	};
	struct opengl_gpu_buffer {

		// General information on the buffer
		// *********************************** //

		GLuint id = 0;        // Its OpenGL index
		GLuint size = 0;      // The number of elements in the buffer (a vec3/uint3 count as 1 element)
		                      //   Note: this is the number of elements (but not the individual number of floats. a vec3/uint3 count as 1 element only)
		GLenum type = 0;      // The type of buffer (ex. GL_ARRAY_BUFFER for VBO, GL_ELEMENT_ARRAY_BUFFER for EBO)



		void bind() const;
		void unbind() const;
		void clear();


		opengl_gpu_buffer_details details;
	};

}
