#include "camera_orbit.hpp"

#include "cgp/01_base/base.hpp"
#include "../common_functions/common_functions.hpp"

namespace cgp
{
	camera_orbit::camera_orbit()
		:orientation_camera(), center_of_rotation({0.0f, 0.0f, 0.0f}), distance_to_center(5.0f)
	{}
	vec3 camera_orbit::position() const
	{
		return orientation_camera * vec3{0.0f, 0.0f, distance_to_center} + center_of_rotation;
	}
	rotation_transform camera_orbit::orientation() const
	{
		return orientation_camera;
	}

	void camera_orbit::manipulator_rotate_arcball(vec2 const& p0, vec2 const& p1)
	{
		rotation_transform const r = trackball_rotation(p0,p1);
		orientation_camera = orientation_camera * inverse(r);
	}
	void camera_orbit::manipulator_rotate_roll_pitch_yaw(float roll, float pitch, float yaw)
	{
		rotation_transform r_roll  = rotation_transform::from_axis_angle({0,0,-1}, roll);
		rotation_transform r_pitch = rotation_transform::from_axis_angle({1,0,0}, pitch);
		rotation_transform r_yaw   = rotation_transform::from_axis_angle({0,1,0}, yaw);

		orientation_camera = orientation_camera * inverse(r_yaw * r_pitch * r_roll);
	}


	void camera_orbit::manipulator_scale_distance_to_center(float magnitude)
	{
		distance_to_center *= (1.0f + magnitude);
		distance_to_center = std::max(distance_to_center, 0.01f);
	}
	void camera_orbit::manipulator_translate_in_plane(vec2 const& tr)
	{
		center_of_rotation -= translation_in_plane(tr, orientation_camera);
	}
	void camera_orbit::manipulator_translate_front(float magnitude)
	{
		center_of_rotation -= magnitude * front();
	}

	void camera_orbit::look_at(vec3 const& eye, vec3 const& center, vec3 const& up)
	{
		frame F = camera_frame_look_at(eye, center, up);
		orientation_camera = F.orientation;
		center_of_rotation = center;
		distance_to_center = norm(eye-center);
	}

}