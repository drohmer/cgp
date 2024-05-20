#pragma once

#include "cgp/09_geometric_transformation/affine/affine.hpp"
#include "cgp/11_mesh/mesh/mesh.hpp"
#include "cgp/13_opengl/opengl.hpp"
#include "cgp/16_drawable/material/material_mesh_drawable_phong/material_mesh_drawable_phong.hpp"
#include "cgp/16_drawable/environment/environment.hpp"

#include <functional>

namespace cgp
{
	// Main structure used to draw a mesh
	struct mesh_drawable
	{
		// Shader data
		// ********************************* //
		static opengl_shader_structure default_shader; // default mesh shader shared by all mesh_drawable 
		opengl_shader_structure shader; // Actual shader (used if defined)

		// Texture image
		// ********************************* //
		static opengl_texture_image_structure default_texture; // default white texture shared by all mesh_drawable
		opengl_texture_image_structure texture; // Actual texture (used if defined)
		std::map<std::string, opengl_texture_image_structure> supplementary_texture; // optional supplementary texture (can be used for multi-texturing)

		// Vertex Buffer Objects
		// ********************************* //
		opengl_vbo_structure vbo_position;
		opengl_vbo_structure vbo_normal;
		opengl_vbo_structure vbo_color;
		opengl_vbo_structure vbo_uv;
		std::vector<opengl_vbo_structure> supplementary_vbo; // optional supplementary vbo (per-vertex or per-instance)

		// Indexed connectivity
		// ********************************* //
		opengl_ebo_structure ebo_connectivity;

		// Vertex Array Object (indicating the VBO organization)
		// See https://www.khronos.org/opengl/wiki/Vertex_Specification for more details
		// ********************************* //
		GLuint vao = 0;

		// ************************************************* //
		// Uniforms parameters 
		//  Parameters sent to the shader automatically when calling draw
		// ************************************************* //

		// Default Model transform
		affine model; // model.translation, model.rotation, model.scaling, model.scaling_xyz

		// Transformation specifically used when using hierarchy
		affine_rts hierarchy_transform_model;

		// Additional generic matrix provided to set the model matrix (initialized to identity)
		mat4 supplementary_model_matrix; 

		// The model matrix sent to the shader is computed as
		//  mat4 M = hierarchy_transform_model.matrix() * supplementary_model_matrix * model.matrix()

		// The material allowing to change the color, and shading parameters
		material_mesh_drawable_phong material;

		// ************************************************* //
		//  Functions of the class
		// ************************************************* //

		// Fill the VBO and VAO of the class using the data provided from the mesh
		void initialize_data_on_gpu(mesh const& data, opengl_shader_structure const& shader = default_shader, opengl_texture_image_structure const& texture = default_texture);

		// Clear the GPU memory from the VBO and VAO data
		void clear();

		// Send the uniforms to the shader (called automatically during the draw stage)
		void send_opengl_uniform(bool expected = true) const;

		// Additional method allowing to fill an additional VBO
		template<typename T>
		void initialize_supplementary_data_on_gpu(numarray<T> const& data, GLuint location_index, GLuint divisor = 0);
		
		// Additional method allowing to update an additional VBO
		template<typename T>
		void update_supplementary_data_on_gpu(numarray<T> const& data, GLuint location_index, int size_elements_update = -1);
	};


	// Main function used to draw a shape.
	//  draw([mesh_drawable], environment);
	void draw(mesh_drawable const& drawable, environment_generic_structure const& environment = environment_generic_structure(), int instance_count=1, bool expected_uniforms=true, uniform_generic_structure const& additional_uniforms = uniform_generic_structure(), GLenum draw_mode=GL_TRIANGLES);

	// Draw the same shape while activating the GL_POLYGON_OFFSET_LINE mode from OpenGL
	void draw_wireframe(mesh_drawable const& drawable, environment_generic_structure const& environment = environment_generic_structure(), vec3 const& color = {0,0,1}, int instance_count = 1, bool expected_uniforms = true, uniform_generic_structure const& additional_uniforms = uniform_generic_structure());


}
