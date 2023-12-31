#pragma once


#include "cgp/05_vec/vec.hpp"
#include "cgp/09_geometric_transformation/geometric_transformation.hpp"
#include "cgp/13_opengl/opengl.hpp"
#include "cgp/16_drawable/environment/environment.hpp"

namespace cgp
{
	enum class curve_drawable_display_type { Curve, Segments };

	struct curve_drawable
	{
		// Type of curve display: contiguous display linking all vertices, or display segments b/w every pair of vertex
		//  - Set display_type to curve_drawable_display_type::Curve for a contiguous curve-like display  
		//      Use glDrawArrays(GL_LINE_STRIP,...) for draw call
		//  - Set display_type to curve_drawable_display_type::Segments for a segment between pairs of vertices
		//      Use glDrawArrays(GL_LINES,...) for draw call
		curve_drawable_display_type display_type = curve_drawable_display_type::Curve;


		// Shader data
		static opengl_shader_structure default_shader; // default curve shader shared by all curve_drawable 
		opengl_shader_structure shader;

		// Per-vertex data
		opengl_vbo_structure vbo_position;

		// VAO indicating the VBO organization
		GLuint vao;

		// Uniform
		affine model;
		vec3 color;

		void initialize_data_on_gpu(numarray<vec3> const& position, opengl_shader_structure const& shader = default_shader);
		void clear();
		void send_opengl_uniform(bool expected = true) const;

	};

	/** Draw the curve_drawable
	*	N_points (default=-1), indicates the number of points displayed. If N_points=-1, display all the points stored in the VBO.*/
	void draw(curve_drawable const& drawable, environment_generic_structure const& environment, int N_points=-1);

}

