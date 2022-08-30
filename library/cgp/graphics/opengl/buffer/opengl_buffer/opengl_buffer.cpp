#include "opengl_buffer.hpp"
#include "../../debug/debug.hpp"


namespace cgp
{
	template <typename T>
	static GLuint opengl_buffer_data_initialize_generic(T const& data, GLuint buffer_type, GLenum draw_type)
	{
		GLuint vbo_index;
		glGenBuffers(1, &vbo_index);                                                       opengl_check
		glBindBuffer(buffer_type, vbo_index);                                              opengl_check
		glBufferData(buffer_type, GLsizeiptr(size_in_memory(data)), ptr(data), draw_type); opengl_check
		glBindBuffer(buffer_type, 0);                                                      opengl_check

		return vbo_index;
	}

	void opengl_gpu_buffer::bind() const
	{
		if(id!=0)
			glBindBuffer(type, id);
	}
	void opengl_gpu_buffer::unbind() const
	{
		glBindBuffer(type, 0);
	}
	void opengl_gpu_buffer::clear()
	{
		glDeleteBuffers(1, &id);  opengl_check;

		id = 0;
		size = 0;
		type = 0;
		details = opengl_gpu_buffer_details();		
	}

}