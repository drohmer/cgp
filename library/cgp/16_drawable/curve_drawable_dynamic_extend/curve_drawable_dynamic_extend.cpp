#include "cgp/01_base/base.hpp"
#include "curve_drawable_dynamic_extend.hpp"

namespace cgp
{
	curve_drawable_dynamic_extend::curve_drawable_dynamic_extend()
		:curve_drawable(), N_valid_points(0)
	{}

	curve_drawable_dynamic_extend& curve_drawable_dynamic_extend::clear()
	{
		curve_drawable::clear();
		N_valid_points = 0;

		return *this;
	}
	curve_drawable_dynamic_extend& curve_drawable_dynamic_extend::initialize_data_on_gpu(int initial_capacity, opengl_shader_structure const& shader)
	{
		numarray<vec3> temp(initial_capacity); // dummy vector of size initial_capacity (values are not used)
		curve_drawable::initialize_data_on_gpu(temp, shader);
		N_valid_points = 0;

		return *this;
	}

	curve_drawable_dynamic_extend& curve_drawable_dynamic_extend::push_back(cgp::vec3 const& p)
	{
		if (int(vbo_position.size) <= int(N_valid_points + 1)) // Need resizing - the VBO is too small
		{
			// Future capacity
			int new_capacity = 2 * vbo_position.size;

			
			opengl_vbo_structure temp_vbo;
			temp_vbo.initialize_data_on_gpu(numarray<vec3>(new_capacity));

			// Copy old VBO into new one
			GLint size_to_copy = vbo_position.size * 3 * sizeof(float);
			glBindBuffer(GL_COPY_READ_BUFFER, vbo_position.id); opengl_check;
			glBindBuffer(GL_COPY_WRITE_BUFFER, temp_vbo.id); opengl_check;
			glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, size_to_copy); opengl_check;
			glBindBuffer(GL_COPY_READ_BUFFER, 0);
			glBindBuffer(GL_COPY_WRITE_BUFFER, 0);

			// remove old vbo
			vbo_position.clear();
			vbo_position = temp_vbo;
			

			// Update the VAO with the new VBO
			glBindVertexArray(vao);     opengl_check
			opengl_set_vao_location(vbo_position, 0);
			glBindVertexArray(0);       opengl_check

		}


		// Add a new position in the VBO
		glBindBuffer(GL_ARRAY_BUFFER, vbo_position.id);                                                   opengl_check;
		glBufferSubData(GL_ARRAY_BUFFER, N_valid_points * 3 * sizeof(float), 3 * sizeof(float), ptr(p));  opengl_check;
		N_valid_points++;


		return *this;
	}

	void draw(curve_drawable_dynamic_extend const& drawable, environment_generic_structure const& environment)
	{
		draw(static_cast<curve_drawable>(drawable), environment, drawable.N_valid_points);
	}
}