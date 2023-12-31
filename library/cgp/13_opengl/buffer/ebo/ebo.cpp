#include "ebo.hpp"
#include "../../debug/debug.hpp"

namespace cgp
{

	void opengl_ebo_structure::initialize_data_on_gpu(numarray<uint3> const& data)
	{

		glGenBuffers(1, &id); opengl_check;
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id); opengl_check;
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, GLsizeiptr(size_in_memory(data)), ptr(data), GL_DYNAMIC_DRAW); opengl_check;
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); opengl_check;

		size = data.size();
		type = GL_ELEMENT_ARRAY_BUFFER;

		details.size_byte = size_in_memory(data);
		details.size_element = 3;
		details.type_element = GL_UNSIGNED_INT;

	}

}