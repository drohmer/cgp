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
		//   If adapt_opengles is true, then the function will attempt to automatically adapt the shader header to be compatible with OpenGL ES
		//    In practice, it will convert the line
		//      # opengl 330 core
		//    into
		//      # opengl 300 es
		//      # precision mediump float;
		// This function raises an error if the shader cannot be loaded succesfully and the program stop indicating an error.
		void load(std::string const& vertex_shader_path, std::string const& fragment_shader_path, bool adapt_opengles=true);

		// Load a new shader from inline text
		// If the shader is loaded successfully, the value load_shader_ok is set to true (if it is not nullptr).
		// If the shader fails to load, the value load_shader_ok is set to false (if it is not nullptr). The program doesn't crash if the shader cannot be loaded.
		void load_from_inline_text(std::string const& vertex_shader_text, std::string const& fragment_shader_text, bool *load_shader_ok=nullptr);

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