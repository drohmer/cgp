#pragma once

#include "cgp/05_vec/vec.hpp"
#include "cgp/06_mat/mat.hpp"

namespace cgp
{
	mat4 projection_perspective(float fov, float aspect, float z_near, float z_far);
	mat4 projection_perspective_inverse(float fov, float aspect, float z_near, float z_far);

	mat4 projection_orthographic(float left, float right, float bottom, float top, float z_near, float z_far);
	mat4 projection_orthographic_inverse(float left, float right, float bottom, float top, float z_near, float z_far);
}