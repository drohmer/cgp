#pragma once

#include "../picking_structure/picking_structure.hpp"
#include "cgp/graphics/camera/camera.hpp"
#include "cgp/geometry/transform/transform.hpp"

#include "cgp/geometry/shape/intersection/intersection.hpp"

namespace cgp
{
	/** Pick a position defined by screen_position on a plane orthogonal to the camera and passing by the specified plane_position*/
	intersection_structure picking_plane_orthogonal_to_camera(vec2 const& screen_position, vec3 const& plane_position, camera_generic_base const& camera, camera_projection_perspective const& projection);
}