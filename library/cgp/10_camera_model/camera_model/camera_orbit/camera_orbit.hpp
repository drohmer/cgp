#pragma once

#include "cgp/09_geometric_transformation/geometric_transformation.hpp"
#include "../camera_generic_base/camera_generic_base.hpp"

namespace cgp
{
	/**
	* Frame matrix =
	* ( orientation | orientation*vec3{0.0f, 0.0f, distance} + center )
	* (      0      |                    1                            )
	*/
	struct camera_orbit : camera_generic_base
	{
		camera_orbit();

		rotation_transform orientation_camera; // Orientation of the camera frame
		vec3 center_of_rotation;               // Position of the center of rotation of the camera
		float distance_to_center;              // Distance between the center of rotation and camera frame position

		vec3 position() const;                  // Position of the camera frame
		rotation_transform orientation() const; // Orientation of the camera frame

	    void manipulator_rotate_arcball(vec2 const& p0, vec2 const& p1);
		void manipulator_rotate_roll_pitch_yaw(float roll, float pitch, float yaw);
	    void manipulator_scale_distance_to_center(float magnitude);
		void manipulator_translate_in_plane(vec2 const& tr);
		void manipulator_translate_front(float magnitude);

		void look_at(vec3 const& eye, vec3 const& center, vec3 const& up);
	};
}