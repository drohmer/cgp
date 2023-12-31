#include "vbo.hpp"
#include "../../debug/debug.hpp"
#include "cgp/01_base/base.hpp"

namespace cgp
{
	static void warning_initialize_non_empty();

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

	void opengl_vbo_structure::initialize_data_on_gpu(numarray<vec3> const& data, GLuint div)
	{
		if(id!=0){
			warning_initialize_non_empty();
		}

		divisor = div;
		id = opengl_buffer_data_initialize_generic(data, GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW);
		size = data.size();
		type = GL_ARRAY_BUFFER;

		details.size_byte = size_in_memory(data);
		details.size_element = 3;
		details.type_element = GL_FLOAT;
	}
	void opengl_vbo_structure::initialize_data_on_gpu(numarray<vec2> const& data, GLuint div)
	{
		if(id!=0){
			warning_initialize_non_empty();
		}

		divisor = div;
		id = opengl_buffer_data_initialize_generic(data, GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW);
		size = data.size();
		type = GL_ARRAY_BUFFER;

		details.size_byte = size_in_memory(data);
		details.size_element = 2;
		details.type_element = GL_FLOAT;
	}
	void opengl_vbo_structure::initialize_data_on_gpu(numarray<vec4> const& data, GLuint div)
	{
		if(id!=0){
			warning_initialize_non_empty();
		}

		divisor = div;
		id = opengl_buffer_data_initialize_generic(data, GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW);
		size = data.size();
		type = GL_ARRAY_BUFFER;

		details.size_byte = size_in_memory(data);
		details.size_element = 4;
		details.type_element = GL_FLOAT;
	}
	void opengl_vbo_structure::update(numarray<vec2> const& data, int size_elements_update)
	{
		assert_cgp(size_elements_update <= data.size(), "Cannot update VBO with more elements than data");
		glBindBuffer(GL_ARRAY_BUFFER, id); opengl_check;
		if (size_elements_update == -1) {
			glBufferSubData(GL_ARRAY_BUFFER, 0, size_in_memory(data), ptr(data));  opengl_check;
		}
		else {
			glBufferSubData(GL_ARRAY_BUFFER, 0, 2 * sizeof(float) * size_elements_update, ptr(data));  opengl_check;
		}
	}
	void opengl_vbo_structure::update(numarray<vec3> const& data, int size_elements_update)
	{
		assert_cgp(size_elements_update <= data.size(), "Cannot update VBO with more elements than data");
		glBindBuffer(GL_ARRAY_BUFFER, id); opengl_check;
		if (size_elements_update == -1) {
			glBufferSubData(GL_ARRAY_BUFFER, 0, size_in_memory(data), ptr(data));  opengl_check;
		}
		else {
			glBufferSubData(GL_ARRAY_BUFFER, 0, 3 * sizeof(float) * size_elements_update, ptr(data));  opengl_check;
		}
	}
	void opengl_vbo_structure::update(numarray<vec4> const& data, int size_elements_update)
	{
		assert_cgp(size_elements_update <= data.size(), "Cannot update VBO with more elements than data");
		glBindBuffer(GL_ARRAY_BUFFER, id); opengl_check;
		if (size_elements_update == -1) {
			glBufferSubData(GL_ARRAY_BUFFER, 0, size_in_memory(data), ptr(data));  opengl_check;
		}
		else {
			glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * sizeof(float) * size_elements_update, ptr(data));  opengl_check;
		}
	}


	void opengl_set_vao_location(opengl_vbo_structure const& vbo, GLuint location_index)
	{
		vbo.bind();
		glEnableVertexAttribArray(location_index); opengl_check
		glVertexAttribPointer(location_index, vbo.details.size_element, vbo.details.type_element, GL_FALSE, 0, nullptr); opengl_check
		vbo.unbind();
		if (vbo.divisor>0) { glVertexAttribDivisor(location_index, vbo.divisor);                                         opengl_check; }
	}


	static void warning_initialize_non_empty()
	{
		std::string warning = "\n";
		warning += "  > You are calling initialize_data_on_gpu on an non-empty VBO (opengl_vbo_structure) \n";
		warning += "In normal condition, you should avoid initializing a new Buffer on an existing one without clearing it - the previously allocated memory on the GPU is going to be lost.\n";
		warning += " - If you to update the values, please call [opengl_vbo_structure].update([new_values]) before calling a new initialization\n";
		warning += " - If you want to clear the memory, please call [opengl_vbo_structure].clear() before calling a new initialization\n";
		warning += " - Note that you should generally not call initialize_data_on_gpu() in the animation loop\n";

		warning_cgp("Calling [vbo].initialize_data_on_gpu() on a non empy VBO", warning);
	}

}
