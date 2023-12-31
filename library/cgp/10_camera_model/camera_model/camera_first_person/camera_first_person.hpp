#pragma once

#include "cgp/09_geometric_transformation/geometric_transformation.hpp"
#include "../camera_generic_base/camera_generic_base.hpp"

namespace cgp
{
	/**
	* Frame matrix =
	* ( orientation_camera |  position_camera )
	* (      0             |     1            )
	*/
	struct camera_first_person : camera_generic_base
	{
		camera_first_person();

		rotation_transform orientation_camera; // Orientation of the camera frame
		vec3 position_camera;                  // Position of the camera frame


		vec3 position() const;                  // Position of the camera frame
		rotation_transform orientation() const; // Orientation of the camera frame

		void manipulator_rotate_roll_pitch_yaw(float roll, float pitch, float yaw);
		void manipulator_translate_in_plane(vec2 const& tr);
		void manipulator_translate_front(float magnitude);

		void look_at(vec3 const& eye, vec3 const& center, vec3 const& up);
	};
}