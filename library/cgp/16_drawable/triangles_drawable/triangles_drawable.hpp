#pragma once

#include "cgp/13_opengl/opengl.hpp"
#include "cgp/11_mesh/mesh.hpp"
#include "cgp/16_drawable/material/material_mesh_drawable_phong/material_mesh_drawable_phong.hpp"
#include "cgp/16_drawable/environment/environment.hpp"
#include "cgp/09_geometric_transformation/affine/affine.hpp"

#include <functional>

namespace cgp
{

	struct triangles_drawable
	{
		// Shader data
		static opengl_shader_structure default_shader; // default mesh shader shared by all triangles_drawable 
		opengl_shader_structure shader;

		// Texture image
		static opengl_texture_image_structure default_texture; // default white texture shared by all triangles_drawable
		opengl_texture_image_structure texture;

		// Per-vertex data
		opengl_vbo_structure vbo_position;
		opengl_vbo_structure vbo_normal;
		opengl_vbo_structure vbo_color;
		opengl_vbo_structure vbo_uv;

		// VAO indicating the VBO organization
		GLuint vao = 0;

		// Uniform
		affine model;
		affine_rts hierarchy_transform_model;

		material_mesh_drawable_phong material;
		int vertex_number; // actual vertex number to draw


		void initialize_data_on_gpu(numarray<vec3> const& position, numarray<vec3> const& normal=numarray<vec3>(), numarray<vec3> const& color=numarray<vec3>(), numarray<vec3> const& uv= numarray<vec3>(), opengl_shader_structure const& shader = default_shader, opengl_texture_image_structure const& texture = default_texture);
		void clear();
		void send_opengl_uniform(bool expected = true) const;

		std::map<std::string, opengl_texture_image_structure> supplementary_texture; // optional supplementary texture (can be used for multi-texturing)
	};

	void draw(triangles_drawable const& drawable, environment_generic_structure const& environment = environment_generic_structure(), uniform_generic_structure const& additional_uniforms = uniform_generic_structure());

	void draw_wireframe(triangles_drawable const& drawable, environment_generic_structure const& environment = environment_generic_structure(), vec3 const& color = {0,0,1}, uniform_generic_structure const& additional_uniforms = uniform_generic_structure());


}
