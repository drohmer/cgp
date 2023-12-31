#pragma once

#include "cgp/05_vec/vec.hpp"
#include "cgp/02_numarray/numarray.hpp"

namespace cgp
{
	struct intersection_structure
	{
		bool valid = false;
		vec3 position = {0,0,0}; // position
		vec3 normal   = {0,0,1}; // normal
	};

	intersection_structure intersection_ray_sphere(vec3 const& ray_origin, vec3 const& ray_direction, vec3 const& sphere_center, float sphere_radius);

	intersection_structure intersection_ray_plane(vec3 const& ray_origin, vec3 const& ray_direction, vec3 const& plane_position, vec3 const& plane_normal);

	intersection_structure intersection_ray_spheres_closest(vec3 const& ray_origin, vec3 const& ray_direction, numarray<vec3> const& sphere_centers, float sphere_radius, int* shape_index=nullptr );

	
}