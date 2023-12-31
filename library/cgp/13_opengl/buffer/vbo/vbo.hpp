#pragma once

#include "../opengl_buffer/opengl_buffer.hpp"
#include "cgp/02_numarray/numarray.hpp"
#include "cgp/05_vec/vec.hpp"
#include "cgp/06_mat/mat.hpp"



namespace cgp
{
	struct opengl_vbo_structure : opengl_gpu_buffer
	{
		void initialize_data_on_gpu(numarray<vec3> const& data, GLuint divisor = 0);
		void initialize_data_on_gpu(numarray<vec2> const& data, GLuint divisor = 0);
		void initialize_data_on_gpu(numarray<vec4> const& data, GLuint divisor = 0);

		/** Re-write data on the VBO. (without re-allocation) in calling glBufferSubData
		* - size_elements_update: 
		*   number of elements to sent from data
		*    -1: send all data (similar to data.size()) 	*/
		void update(numarray<vec2> const& data, int size_elements_update = -1);
		void update(numarray<vec3> const& data, int size_elements_update = -1);
		void update(numarray<vec4> const& data, int size_elements_update = -1);

		GLuint divisor;
	};

	/** Call glVertexAttribPointer and set the correspondance between VBO and the location in the shader */
	void opengl_set_vao_location(opengl_vbo_structure const& vbo, GLuint location_index);

}
