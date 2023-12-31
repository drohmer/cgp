#include "camera_first_person.hpp"

#include "cgp/01_base/base.hpp"
#include "../common_functions/common_functions.hpp"

namespace cgp
{

	camera_first_person::camera_first_person()
		:orientation_camera(), position_camera()
	{}
	vec3 camera_first_person::position() const{
		return position_camera;
	}
	rotation_transform camera_first_person::orientation() const{
		return orientation_camera;
	}


	void camera_first_person::manipulator_rotate_roll_pitch_yaw(float roll, float pitch, float yaw)
	{
		rotation_transform r_roll  = rotation_transform::from_axis_angle({0,0,-1}, roll);
		rotation_transform r_pitch = rotation_transform::from_axis_angle({1,0,0}, pitch);
		rotation_transform r_yaw   = rotation_transform::from_axis_angle({0,1,0}, yaw);

		orientation_camera = orientation_camera * inverse(r_yaw * r_pitch * r_roll);
	}



	void camera_first_person::manipulator_translate_in_plane(vec2 const& tr)
	{
		position_camera -= translation_in_plane(tr, orientation_camera);
	}
	void camera_first_person::manipulator_translate_front(float magnitude)
	{
		position_camera += magnitude * front();
	}

	void camera_first_person::look_at(vec3 const& eye, vec3 const& center, vec3 const& up)
	{
		frame F = camera_frame_look_at(eye, center, up);
		orientation_camera = F.orientation;
		position_camera = eye;
	}

}