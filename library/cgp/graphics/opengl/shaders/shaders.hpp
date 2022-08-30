#pragma once

#include "cgp/opengl_include.hpp"

#include "cache_uniform_location/cache_uniform_location.hpp"


namespace cgp
{
	struct opengl_shader_structure
	{
		// The OpenGL ID designating the shader.
		//  Default set to 0 : indicates that no shader is set
		GLuint id = 0;


		// Load a new shader from filepath
		//  Expect to load a new shader on an empty structure (otherwise the previous shader is not automatically destroyed from memory)
		void load(std::string const& vertex_shader_path, std::string const& fragment_shader_path);

		// Load a new shader from inline text
		void load_from_inline_text(std::string const& vertex_shader_text, std::string const& fragment_shader_text);

		// Query the location of a uniform variable using the cache system
		GLint query_uniform_location(std::string const& uniform_name) const;

		// Clear the cache system
		void clear_cache_uniform_location();

		// Debug information of the current cache storage between uniform name and location
		static std::string debug_dump_cache_uniform_location();

	private:
		// Global caching system to store the correspondance between a uniform name and its location for a given shader
		// Usage: location = cache_uniform_location.query(shaderID, uniformName)
		//        Return -1 if the uniformName doesn't exists
		//  The cache is updated automatically when using the function opengl_get_location_uniform
		//  Note that this cache is shader through all instances of shader_structure (to take care in case of parallelism)
		static cache_uniform_location_structure cache_uniform_location;
	};




}