#pragma once

#include "../picking_structure/picking_structure.hpp"
#include "cgp/02_numarray/numarray/numarray.hpp"
#include "cgp/10_camera_model/camera_model.hpp"
#include "cgp/09_geometric_transformation/geometric_transformation.hpp"

namespace cgp
{
	/** Picking through a set of spheres with specified centers and given radius */
	picking_structure picking_spheres(vec2 const& screen_click, numarray<vec3> const& spheres_centers, float spheres_radius, camera_generic_base const& camera, camera_projection_perspective const& projection);

	/** Compute picking of a mesh vertex assuming that each vertex is a sphere of specified radius */
	picking_structure picking_mesh_vertex_as_sphere(vec2 const& screen_click, numarray<vec3> const& vertex_position, numarray<vec3> const& vertex_normal, float picking_distance, camera_generic_base const& camera, camera_projection_perspective const& projection);
}