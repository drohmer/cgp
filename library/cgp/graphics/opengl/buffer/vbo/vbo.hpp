#pragma once

#include "../opengl_buffer/opengl_buffer.hpp"
#include "cgp/core/array/array.hpp"
#include "cgp/geometry/vec/vec.hpp"
#include "cgp/geometry/mat/mat.hpp"



namespace cgp
{
	struct opengl_vbo_structure : opengl_gpu_buffer
	{
		void initialize_data_on_gpu(numarray<vec3> const& data);
		void initialize_data_on_gpu(numarray<vec2> const& data);
		void initialize_data_on_gpu(numarray<vec4> const& data);

		/** Re-write data on the VBO. (without re-allocation) in calling glBufferSubData
		* - size_elements_update: 
		*   number of elements to sent from data
		*    -1: send all data (similar to data.size()) 	*/
		void update(numarray<vec2> const& data, int size_elements_update = -1);
		void update(numarray<vec3> const& data, int size_elements_update = -1);
		void update(numarray<vec4> const& data, int size_elements_update = -1);
	};

	/** Call glVertexAttribPointer and set the correspondance between VBO and the location in the shader */
	void opengl_set_vao_location(opengl_vbo_structure const& vbo, GLuint location_index);

}