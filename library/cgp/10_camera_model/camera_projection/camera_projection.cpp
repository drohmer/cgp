#include "camera_projection.hpp"

#include "cgp/01_base/base.hpp"


namespace cgp {
	
	mat4 camera_projection_perspective::matrix() const
	{
		return projection_perspective(field_of_view, aspect_ratio, depth_min, depth_max);
	}

	mat4 camera_projection_perspective::matrix_inverse() const
	{
		return projection_perspective_inverse(field_of_view, aspect_ratio, depth_min, depth_max);
	}

	mat4 camera_projection_orthographic::matrix() const
	{
		return projection_orthographic(left*aspect_ratio, right*aspect_ratio, bottom, top, z_min, z_max);
	}

	mat4 camera_projection_orthographic::matrix_inverse() const
	{
		return projection_orthographic_inverse(left*aspect_ratio, right*aspect_ratio, bottom, top, z_min, z_max);
	}

}