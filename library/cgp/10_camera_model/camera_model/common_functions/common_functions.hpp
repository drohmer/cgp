#pragma once

#include "cgp/09_geometric_transformation/geometric_transformation.hpp"

namespace cgp
{
	vec3 translation_in_plane(vec2 const& translation, rotation_transform const& plane_orientation);
	vec3 trackball_projection(float x, float y);
	rotation_transform trackball_rotation(vec2 const& p0, vec2 const& p1);
}