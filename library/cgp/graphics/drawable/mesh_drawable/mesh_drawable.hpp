#pragma once

#include "cgp/graphics/opengl/opengl.hpp"
#include "cgp/geometry/shape/mesh/mesh.hpp"
#include "cgp/graphics/drawable/material/material_mesh_drawable_phong/material_mesh_drawable_phong.hpp"
#include "cgp/graphics/drawable/environment/environment.hpp"
#include "cgp/geometry/transform/affine/affine.hpp"

#include <functional>

namespace cgp
{

	struct mesh_drawable
	{
		// Shader data
		static opengl_shader_structure default_shader; // default mesh shader shared by all mesh_drawable 
		opengl_shader_structure shader;

		// Texture image
		static opengl_texture_image_structure default_texture; // default white texture shared by all mesh_drawable
		opengl_texture_image_structure texture;

		// Per-vertex data
		opengl_vbo_structure vbo_position;
		opengl_vbo_structure vbo_normal;
		opengl_vbo_structure vbo_color;
		opengl_vbo_structure vbo_uv;

		// Indexed connectivity
		opengl_ebo_structure ebo_connectivity;

		// VAO indicating the VBO organization
		GLuint vao = 0;


		// Uniform
		affine model;
		affine_rts hierarchy_transform_model;

		material_mesh_drawable_phong material;


		void initialize_data_on_gpu(mesh const& data, opengl_shader_structure const& shader = default_shader, opengl_texture_image_structure const& texture = default_texture);
		void clear();
		void send_opengl_uniform(bool expected = true) const;

		std::map<std::string, opengl_texture_image_structure> supplementary_texture; // optional supplementary texture (can be used for multi-texturing)
	};

	void draw(mesh_drawable const& drawable, environment_generic_structure const& environment = environment_generic_structure(), uniform_generic_structure const& additional_uniforms = uniform_generic_structure(), GLenum draw_mode=GL_TRIANGLES);

	void draw_wireframe(mesh_drawable const& drawable, environment_generic_structure const& environment = environment_generic_structure(), vec3 const& color = {0,0,1}, uniform_generic_structure const& additional_uniforms = uniform_generic_structure());


}
