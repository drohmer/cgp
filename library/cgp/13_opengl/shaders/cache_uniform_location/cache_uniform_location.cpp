#include "cache_uniform_location.hpp"

#include "cgp/01_base/base.hpp"
#include "cgp/13_opengl/debug/debug.hpp"

#include <iostream>


namespace cgp
{
    GLint cache_uniform_location_structure::query(GLuint shaderID, std::string const& uniformName)
    {
        // Sanity check
        assert_cgp(shaderID != 0, "Try to query uniform " + uniformName + " on unspecified shader (shader index = 0).");

        // Check if shaderID is already recorded
        auto shaderID_it = cache_data.find(shaderID);

        // If new shader, then create an empty map
        if (shaderID_it == cache_data.end())
            cache_data[shaderID] = std::map<std::string, GLint>();

        // Check if uniformName is already recorded
        std::map<std::string, GLint>& cacheShaderQuery = cache_data[shaderID];
        auto uniformLoc_it = cacheShaderQuery.find(uniformName);

        // If found, return the cached value
        if (uniformLoc_it != cacheShaderQuery.end())
            return uniformLoc_it->second;

        // Else: the name is not found
        // Then we query the location using glGetUniformLocation in the shader
        GLint const location = glGetUniformLocation(shaderID, uniformName.c_str()); opengl_check;

        // Add the location in the cache system
        //  Note: location == -1 if glGetUniformLocation cannot find the variable
        cacheShaderQuery[uniformName] = location;

        return location;
    }

    std::string str(cache_uniform_location_structure const& cache)
    {
        std::string s;
        for (auto it1 : cache.cache_data) {
            for (auto it2 : it1.second) {
                s += str(it1.first) + " : " + str(it2.first) + " -> " + str(it2.second) + "\n";
            }
            s += "\n";
        }
        return s;
    }

    std::ostream& operator<<(std::ostream& s, cache_uniform_location_structure const& cache)
    {
        s << str(cache);
        return s;
    }

}