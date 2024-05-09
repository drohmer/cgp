#pragma once

#include <string>
#include "cgp/opengl_include.hpp"
#include "cgp/05_vec/vec.hpp"
#include "cgp/06_mat/mat.hpp"
#include "cgp/13_opengl/shaders/shaders.hpp"
#include "cgp/13_opengl/texture/texture.hpp"

namespace cgp
{

	// Generic structure to store a set of uniforms
	struct uniform_generic_structure
	{
		std::map<std::string, int> uniform_int;
		std::map<std::string, float> uniform_float;

		std::map<std::string, vec2> uniform_vec2;
		std::map<std::string, vec3> uniform_vec3;
		std::map<std::string, vec4> uniform_vec4;

		std::map<std::string, mat2> uniform_mat2;
		std::map<std::string, mat3> uniform_mat3;
		std::map<std::string, mat4> uniform_mat4;


		void send_opengl_uniform(opengl_shader_structure const& shader, bool expected = true) const;
	};



	void opengl_uniform(opengl_shader_structure const& shader, std::string const& name, int value, bool expected = true);
	void opengl_uniform(opengl_shader_structure const& shader, std::string const& name, GLuint value, bool expected = true);
	void opengl_uniform(opengl_shader_structure const& shader, std::string const& name, float value, bool expected = true);

	void opengl_uniform(opengl_shader_structure const& shader, std::string const& name, vec2 const& value, bool expected = true);
	void opengl_uniform(opengl_shader_structure const& shader, std::string const& name, vec3 const& value, bool expected = true);
	void opengl_uniform(opengl_shader_structure const& shader, std::string const& name, vec4 const& value, bool expected = true);

	void opengl_uniform(opengl_shader_structure const& shader, std::string const& name, float x, float y, bool expected = true);
	void opengl_uniform(opengl_shader_structure const& shader, std::string const& name, float x, float y, float z, bool expected = true);
	void opengl_uniform(opengl_shader_structure const& shader, std::string const& name, float x, float y, float z, float w, bool expected = true);

	void opengl_uniform(opengl_shader_structure const& shader, std::string const& name, mat4 const& m, bool expected = true);
	void opengl_uniform(opengl_shader_structure const& shader, std::string const& name, mat3 const& m, bool expected = true);
	void opengl_uniform(opengl_shader_structure const& shader, std::string const& name, mat2 const& m, bool expected = true);

}

