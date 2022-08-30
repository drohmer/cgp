#include "vbo.hpp"
#include "../../debug/debug.hpp"
#include "cgp/core/core.hpp"

namespace cgp
{
	template <int N>
	static GLuint opengl_buffer_data_initialize_generic(numarray<numarray_stack<float,N> > const& data, GLuint buffer_type, GLenum draw_type)
	{
		GLuint vbo_index;
		glGenBuffers(1, &vbo_index);                                                       opengl_check;
		glBindBuffer(buffer_type, vbo_index);                                              opengl_check;
		glBufferData(buffer_type, GLsizeiptr(size_in_memory(data)), ptr(data), draw_type); opengl_check;
		glBindBuffer(buffer_type, 0);                                                      opengl_check;

		return vbo_index;
	}

	void opengl_vbo_structure::initialize_data_on_gpu(numarray<vec3> const& data)
	{
		id = opengl_buffer_data_initialize_generic(data, GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW);
		size = data.size();
		type = GL_ARRAY_BUFFER;

		details.size_byte = size_in_memory(data);
		details.size_element = 3;
		details.type_element = GL_FLOAT;
	}
	void opengl_vbo_structure::initialize_data_on_gpu(numarray<vec2> const& data)
	{
		id = opengl_buffer_data_initialize_generic(data, GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW);
		size = data.size();
		type = GL_ARRAY_BUFFER;

		details.size_byte = size_in_memory(data);
		details.size_element = 2;
		details.type_element = GL_FLOAT;
	}
	void opengl_vbo_structure::initialize_data_on_gpu(numarray<vec4> const& data)
	{
		id = opengl_buffer_data_initialize_generic(data, GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW);
		size = data.size();
		type = GL_ARRAY_BUFFER;

		details.size_byte = size_in_memory(data);
		details.size_element = 4;
		details.type_element = GL_FLOAT;
	}

	void opengl_vbo_structure::update(numarray<vec2> const& data)
	{
		glBindBuffer(GL_ARRAY_BUFFER, id); opengl_check;
		glBufferSubData(GL_ARRAY_BUFFER, 0, size_in_memory(data), ptr(data));  opengl_check;
	}
	void opengl_vbo_structure::update(numarray<vec3> const& data)
	{
		glBindBuffer(GL_ARRAY_BUFFER, id); opengl_check;
		glBufferSubData(GL_ARRAY_BUFFER, 0, size_in_memory(data), ptr(data));  opengl_check;
	}
	void opengl_vbo_structure::update(numarray<vec4> const& data)
	{
		glBindBuffer(GL_ARRAY_BUFFER, id); opengl_check;
		glBufferSubData(GL_ARRAY_BUFFER, 0, size_in_memory(data), ptr(data));  opengl_check;
	}


	void opengl_set_vao_location(opengl_vbo_structure const& vbo, GLuint location_index)
	{
		vbo.bind();
		glEnableVertexAttribArray(location_index); opengl_check
		glVertexAttribPointer(location_index, vbo.details.size_element, vbo.details.type_element, GL_FALSE, 0, nullptr); opengl_check
		vbo.unbind();
	}

}