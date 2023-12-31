#include "picking_spheres.hpp"

#include "cgp/12_shape/intersection/intersection.hpp"

namespace cgp
{
	picking_structure picking_spheres(vec2 const& screen_click, numarray<vec3> const& spheres_centers, float spheres_radius, camera_generic_base const& camera, camera_projection_perspective const& projection)
	{
		picking_structure picking;

		picking.ray_direction = camera_ray_direction(camera.matrix_frame(), projection.matrix_inverse(), screen_click);
		picking.ray_origin = camera.position();
		picking.screen_clicked = screen_click;

		intersection_structure intersection = intersection_ray_spheres_closest(picking.ray_origin, picking.ray_direction, spheres_centers, spheres_radius, &picking.index);

		if (intersection.valid == true) {
			picking.active = true;
			picking.position = intersection.position;
			picking.normal = normalize(picking.position - spheres_centers[picking.index]);
		}

		return picking;
	}

	picking_structure picking_mesh_vertex_as_sphere(vec2 const& screen_click, numarray<vec3> const& vertex_position, numarray<vec3> const& vertex_normal, float picking_distance, camera_generic_base const& camera, camera_projection_perspective const& projection)
	{
		picking_structure picking;

		picking.ray_direction = camera_ray_direction(camera.matrix_frame(), projection.matrix_inverse(), screen_click);
		picking.ray_origin = camera.position();
		picking.screen_clicked = screen_click;

		intersection_structure intersection = intersection_ray_spheres_closest(picking.ray_origin, picking.ray_direction, vertex_position, picking_distance, &picking.index);

		if (intersection.valid == true) {
			picking.active = true;
			picking.position = intersection.position;
			picking.normal = vertex_normal[picking.index];
		}

		return picking;
	}
}