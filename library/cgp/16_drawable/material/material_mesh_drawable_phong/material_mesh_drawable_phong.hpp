#pragma once

#include "cgp/02_numarray/numarray_stack/numarray_stack.hpp"
#include "cgp/opengl_include.hpp"

namespace cgp
{
	struct phong_parameters {
		float ambient = 0.3f;
		float diffuse = 0.6f;
		float specular = 0.3f;
		float specular_exponent = 64.0f;
	};
	struct texture_settings_parameters {
		bool active = true;
		bool inverse_v = true;
		bool two_sided = false;
	};
	struct material_mesh_drawable_phong
	{
		vec3 color = vec3{ 1.0f,1.0f,1.0f };    // Global RGB-color of the object
		float alpha = 1.0;                      // Alpha component of the uniform color RGBA (can be used for transparency)

		phong_parameters phong;                       // Phong parameters
		texture_settings_parameters texture_settings; // Specific settings for the texture (the texture id is stored directly in the mesh_drawable)

		void send_opengl_uniform(opengl_shader_structure const& shader, bool expected = true) const;
	};

	//void opengl_uniform(opengl_shader_structure const& shader, material_mesh_drawable_phong const& material, bool expected = true);
}

