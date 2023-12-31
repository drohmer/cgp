#include "cgp/01_base/base.hpp"
#include "cgp/13_opengl/opengl.hpp"

#include "material_mesh_drawable_phong.hpp"



namespace cgp
{
	void material_mesh_drawable_phong::send_opengl_uniform(opengl_shader_structure const& shader, bool expected) const
	{
		opengl_uniform(shader, "material.color", color, expected);
		opengl_uniform(shader, "material.alpha", alpha, expected);

		opengl_uniform(shader, "material.phong.ambient", phong.ambient, expected);
		opengl_uniform(shader, "material.phong.diffuse", phong.diffuse, expected);
		opengl_uniform(shader, "material.phong.specular", phong.specular, expected);
		opengl_uniform(shader, "material.phong.specular_exponent", phong.specular_exponent, expected);

		opengl_uniform(shader, "material.texture_settings.use_texture", texture_settings.active, expected);
		opengl_uniform(shader, "material.texture_settings.texture_inverse_v", texture_settings.inverse_v, expected);
		opengl_uniform(shader, "material.texture_settings.two_sided", texture_settings.two_sided, expected);
	}

}