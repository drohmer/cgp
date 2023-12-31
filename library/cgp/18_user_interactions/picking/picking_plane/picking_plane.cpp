#include "picking_plane.hpp"



namespace cgp
{
	intersection_structure picking_plane_orthogonal_to_camera(vec2 const& screen_position, vec3 const& plane_position, camera_generic_base const& camera, camera_projection_perspective const& projection)
	{

		vec3 const camera_direction = camera.front();
		vec3 const ray_direction = camera_ray_direction(camera.matrix_frame(), projection.matrix_inverse(), screen_position);
		vec3 const ray_origin = camera.position();
		intersection_structure intersection = intersection_ray_plane(ray_origin, ray_direction, plane_position, camera_direction);

		return intersection;
	}
}