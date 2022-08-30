#include "common_functions.hpp"

namespace cgp
{
	vec3 translation_in_plane(vec2 const& translation, rotation_transform const& plane_orientation)
	{
		return plane_orientation * vec3(translation, 0.0f);
	}

	vec3 trackball_projection(float x, float y)
	{
		const float d = std::sqrt(x * x + y * y);
		float z = 0;
		if (d < 1.0f / std::sqrt(2.0f))
			z = std::sqrt(1.0f - d * d);
		else
			z = 1.0f / (2 * d);

		return { x, y, z };
	}

	rotation_transform trackball_rotation(vec2 const& p0, vec2 const& p1)
	{
		rotation_transform R;
		const float d = norm(p1-p0);
		if( d > 1e-4f )
		{
			vec3 const s0 = trackball_projection(p0.x, p0.y);
			vec3 const s1 = trackball_projection(p1.x, p1.y);
		
			R = rotation_transform::from_vector_transform(normalize(s0), normalize(s1));
		}
		return R;
	}
}