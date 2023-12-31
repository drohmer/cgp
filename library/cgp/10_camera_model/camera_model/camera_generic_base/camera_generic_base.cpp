#include "cgp/01_base/base.hpp"
#include "camera_generic_base.hpp"




namespace cgp
{
	frame camera_generic_base::to_frame() const
	{
		return frame(orientation(), position());
	}
	mat4 camera_generic_base::matrix_view() const
	{
		return inverse(to_frame()).matrix();
	}
	mat4 camera_generic_base::matrix_frame() const
	{
		return to_frame().matrix();
	}

	vec3 camera_generic_base::front() const
	{
		return -orientation().matrix_col_z();
	}
	vec3 camera_generic_base::up() const
	{
		return orientation().matrix_col_y();
	}
	vec3 camera_generic_base::right() const
	{
		return orientation().matrix_col_x();
	}


	vec3 camera_ray_direction(mat4 const& camera_frame, mat4 const& perspective_inverse, vec2 const& screen_position)
	{
		vec4 const direction_eye_space = perspective_inverse * vec4(screen_position, -1.0f, 1.0f);
		vec4 const direction_4d = camera_frame * vec4(direction_eye_space.xy(), -1.0f, 0.0f);

		vec3 const direction = normalize(direction_4d.xyz());
		return direction;
	}

	frame camera_frame_look_at(vec3 const& eye, vec3 const& center, vec3 const& up)
	{
		vec3 const uz = normalize(eye-center);
		vec3 const ux = normalize(cross(up, uz));
		vec3 const uy = cross(uz,ux);
		rotation_transform const R = rotation_transform::from_frame_transform({1,0,0}, {0,1,0}, ux, uy);

		return frame(R, eye);
	}
}