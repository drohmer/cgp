#pragma once

#include "cgp/opengl_include.hpp"

#include <string>
#include <map>

namespace cgp
{
	// Caching system to store the correspondance between a uniform name and its location for a given shader
	// Usage: location = cache_uniform_location.query(shaderID, uniformName)
	struct cache_uniform_location_structure
	{
		std::map<GLuint, std::map<std::string, GLint> > cache_data;

		// Return the location of the uniform in the shader designated by shaderID and update the caching system
		//  Query glGetUniformLocation the first time the variable is queried and save it.
		//  The following times, the variable is read from the cache without requiring access to glGetUniformLocation (that can slow down rendering pipeline)
		//  If uniformName is not found return (and cache) the value -1.
		GLint query(GLuint shaderID, std::string const& uniformName);

	};

	std::string str(cache_uniform_location_structure const& cache);
	std::ostream& operator<<(std::ostream& s, cache_uniform_location_structure const& cache);

}