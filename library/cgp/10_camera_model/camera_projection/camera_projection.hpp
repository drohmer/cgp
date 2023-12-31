#pragma once

#include "cgp/09_geometric_transformation/geometric_transformation.hpp"

namespace cgp
{
	// perspective model
	struct camera_projection_perspective
	{
		float field_of_view = 50.0f * Pi/180;
		float aspect_ratio = 1.0f;
		float depth_min = 0.01f;
		float depth_max = 1000;

		mat4 matrix() const;
		mat4 matrix_inverse() const;
	};

	struct camera_projection_orthographic
	{
		float left = -1.0f;
		float right = 1.0f;
		float bottom = -1.0f;
		float top = 1.0f;
		float z_min = -10.0f;
		float z_max = 10.0f;
		float aspect_ratio = 1.0f;

		mat4 matrix() const;
		mat4 matrix_inverse() const;
	};
}

	
/*
	struct perspective_parameters {
		float field_of_view =  50.0f * 3.14f/180;
		float aspect_ratio  =   1.0f;
		float depth_min     =   0.1f;
		float depth_max     = 500.0f;
	};

	struct orthographic_parameters {
		float left   = -1;
		float right  =  1;
		float bottom = -1;
		float top    =  1;
		float back   = -1;
		float front  =  1;
	};

	enum struct camera_perspective_type {perspective, orthographic};
	struct camera_projection
	{
		static camera_projection build_perspective(float field_of_view, float aspect_ratio, float depth_min, float depth_max);
		static camera_projection build_orthographic(float left, float right, float bottom, float top, float back, float front, float aspect_ratio=1.0f);

		camera_perspective_type type = camera_perspective_type::perspective;
		perspective_parameters perspective_data;
		orthographic_parameters orthographic_data;
		float aspect_ratio = 1.0f;


		mat4 matrix() const;
		mat4 matrix_inverse() const;
		
		void update_aspect_ratio(float aspect_ratio);
	};

}
*/