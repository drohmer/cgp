
#include "uniform.hpp"

#include "cgp/01_base/base.hpp"
#include "cgp/13_opengl/debug/debug.hpp"


namespace cgp
{
	static bool check_location(GLint location, std::string const& name, GLuint shader, bool expected)
	{
		if (location == -1 && expected == true)
		{
			std::string const error_str = "Try to send uniform variable [" + name + "] to a shader that doesn't use it.\n Either change the uniform variable to expected=false, or correct the associated shader (id=" + str(shader) + ").";
#ifdef CHECK_OPENGL_UNIFORM_STRICT
			error_cgp(error_str);
#else
			warning_cgp(error_str,"");
#endif
		}
		if(location == -1)
			return false;
		else
			return true;

	}


	void opengl_uniform(opengl_shader_structure const& shader, std::string const& name, int value, bool expected)
	{
		GLint const location = shader.query_uniform_location(name);
		if (check_location(location, name, shader.id, expected)) {
			glUniform1i(location, value); opengl_check;
		}
	}

	void opengl_uniform(opengl_shader_structure const& shader, std::string const& name, GLuint value, bool expected)
	{
		GLint const location = shader.query_uniform_location(name);
		if (check_location(location, name, shader.id, expected)) {
			glUniform1i(location, value); opengl_check;
		}

	}
	void opengl_uniform(opengl_shader_structure const& shader, std::string const& name, float value, bool expected)
	{
		GLint const location = shader.query_uniform_location(name);
		if (check_location(location, name, shader.id, expected)) {
			glUniform1f(location, value); opengl_check;
		}
	}
	void opengl_uniform(opengl_shader_structure const& shader, std::string const& name, vec2 const& value, bool expected)
	{
		GLint const location = shader.query_uniform_location(name);
		if (check_location(location, name, shader.id, expected)) {
			glUniform2f(location, value.x, value.y); opengl_check;
		}
	}
	void opengl_uniform(opengl_shader_structure const& shader, std::string const& name, vec3 const& value, bool expected)
	{
		GLint const location = shader.query_uniform_location(name);
		if (check_location(location, name, shader.id, expected)) {
			glUniform3f(location, value.x, value.y, value.z); opengl_check;
		}
	}
	void opengl_uniform(opengl_shader_structure const& shader, std::string const& name, vec4 const& value, bool expected)
	{
		GLint const location = shader.query_uniform_location(name);
		if (check_location(location, name, shader.id, expected)) {
			glUniform4f(location, value.x, value.y, value.z, value.w); opengl_check;
		}
	}
	void opengl_uniform(opengl_shader_structure const& shader, std::string const& name, float x, float y, bool expected)
	{
		GLint const location = shader.query_uniform_location(name);
		if (check_location(location, name, shader.id, expected)) {
			glUniform2f(location, x, y);  opengl_check;
		}
	}
	void opengl_uniform(opengl_shader_structure const& shader, std::string const& name, float x, float y, float z, bool expected)
	{
		GLint const location = shader.query_uniform_location(name);
		if (check_location(location, name, shader.id, expected)) {
			glUniform3f(location, x, y, z);  opengl_check;
		}
	}
	void opengl_uniform(opengl_shader_structure const& shader, std::string const& name, float x, float y, float z, float w, bool expected)
	{
		GLint const location = shader.query_uniform_location(name);
		if (check_location(location, name, shader.id, expected)) {
			glUniform4f(location, x, y, z, w);  opengl_check;
		}
	}
	void opengl_uniform(opengl_shader_structure const& shader, std::string const& name, mat4 const& m, bool expected)
	{
		GLint const location = shader.query_uniform_location(name);
		if (check_location(location, name, shader.id, expected)) {
			glUniformMatrix4fv(location, 1, GL_TRUE, ptr(m));  opengl_check;
		}
	}
	void opengl_uniform(opengl_shader_structure const& shader, std::string const& name, mat3 const& m, bool expected)
	{
		GLint const location = shader.query_uniform_location(name);
		if (check_location(location, name, shader.id, expected)) {
			glUniformMatrix3fv(location, 1, GL_TRUE, ptr(m)); opengl_check;
		}
	}
	void opengl_uniform(opengl_shader_structure const& shader, std::string const& name, mat2 const& m, bool expected)
	{
		GLint const location = shader.query_uniform_location(name);
		if (check_location(location, name, shader.id, expected)) {
			glUniformMatrix2fv(location, 1, GL_TRUE, ptr(m)); opengl_check;
		}
	}



	void uniform_generic_structure::send_opengl_uniform(opengl_shader_structure const& shader, bool expected) const
	{
		for (auto const& data : uniform_int)
			opengl_uniform(shader, data.first, data.second, expected);
		for (auto const& data : uniform_float)
			opengl_uniform(shader, data.first, data.second, expected);

		for (auto const& data : uniform_vec2)
			opengl_uniform(shader, data.first, data.second, expected);
		for (auto const& data : uniform_vec3)
			opengl_uniform(shader, data.first, data.second, expected);
		for (auto const& data : uniform_vec4)
			opengl_uniform(shader, data.first, data.second, expected);

		for (auto const& data : uniform_mat2)
			opengl_uniform(shader, data.first, data.second, expected);
		for (auto const& data : uniform_mat3)
			opengl_uniform(shader, data.first, data.second, expected);
		for (auto const& data : uniform_mat4)
			opengl_uniform(shader, data.first, data.second, expected);

	}

}