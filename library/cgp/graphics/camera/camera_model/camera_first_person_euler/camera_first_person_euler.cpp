#include "camera_first_person_euler.hpp"

#include "cgp/core/base/base.hpp"
#include "../common_functions/common_functions.hpp"

namespace cgp
{

	camera_first_person_euler::camera_first_person_euler()
		:roll(0), pitch(0), yaw(0), axis_of_rotation({ 0,1,0 }), position_camera()
	{}

	vec3 camera_first_person_euler::position() const{
		return position_camera;
	}
	rotation_transform camera_first_person_euler::orientation() const{
		rotation_transform r_roll = rotation_transform::from_axis_angle({ 0,0,-1 }, roll);
		rotation_transform r_pitch = rotation_transform::from_axis_angle({ 1,0,0 }, pitch);
		rotation_transform r_yaw = rotation_transform::from_axis_angle({ 0,1,0 }, yaw);
		rotation_transform r_axis = rotation_transform::from_vector_transform({ 0,1,0 }, axis_of_rotation);

		return r_axis * r_yaw * r_pitch * r_roll;
	}


	void camera_first_person_euler::manipulator_rotate_roll_pitch_yaw(float d_roll, float d_pitch, float d_yaw)
	{
		roll += d_roll;
		pitch += d_pitch;
		yaw += d_yaw;
	}

	void camera_first_person_euler::manipulator_translate_in_plane(vec2 const& tr)
	{
		position_camera -= translation_in_plane(tr, orientation());
	}
	void camera_first_person_euler::manipulator_translate_front(float magnitude)
	{
		position_camera -= magnitude * front();
	}

	void camera_first_person_euler::set_rotation_axis(vec3 axis)
	{
		axis_of_rotation = axis;
	}
	void camera_first_person_euler::manipulator_twist_rotation_axis(float magnitude)
	{
		// Compute the front direction after applying the current rotation (when there is no pitch)
		rotation_transform r_axis = rotation_transform::from_vector_transform({ 0,1,0 }, axis_of_rotation);
		vec3 front = r_axis * vec3(0, 0, 1);

		// Rotates the current axis_of_rotation around the direction of the new front vector R*(0,0,1)
		axis_of_rotation = rotation_transform::from_axis_angle(front, magnitude) * axis_of_rotation;
	}

	void camera_first_person_euler::look_at(vec3 const& eye, vec3 const& center)
	{
		position_camera = eye;
		float const distance_to_center = norm(eye - center);

		rotation_transform r_axis = rotation_transform::from_vector_transform({ 0,1,0 }, axis_of_rotation);

		// The direction of the front vector (/or a point on the unit sphere)
		vec3  p_global = (eye - center) / distance_to_center;

		// Front vector (/point on the unit sphere) in the local reference frame where the axis is y
		vec3  p = inverse(r_axis) * p_global;

		/*
		* yaw * pitch = (  cos(yaw) 0  sin(yaw) )  ( 1     0           0      )
		*               (      0    1     0     )  ( 0 cos(pitch) -sin(pitch) )
		*               ( -sin(yaw) 0  cos(yaw) )  ( 0 sin(pitch)  cos(pitch) )
		*
		*             = (  cos(yaw)  sin(yaw)sin(pitch)  sin(yaw)cos(pitch) )
		*               (     0          cos(pitch)          -sin(pitch)    )
		*               ( -sin(yaw)  cos(yaw)sin(pitch)  cos(yaw)cos(pitch) )
		*
		* We solve:
		*    (x y z) = yaw * pitch * (0 0 1) = (sin(yaw)cos(pitch)  -sin(pitch)  cos(yaw)cos(pitch) )
		* => x/z = tan(yaw) ; y / sqrt(z^2+x^2) = tan(pitch)
		*/

		// Trigonometric inverse
		pitch = -std::atan2(p.y, std::sqrt(p.z * p.z + p.x * p.x));
		yaw = std::atan2(p.x, p.z);
	}

}