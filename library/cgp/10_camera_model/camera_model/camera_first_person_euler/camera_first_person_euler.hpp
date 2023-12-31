#pragma once

#include "cgp/09_geometric_transformation/geometric_transformation.hpp"
#include "../camera_generic_base/camera_generic_base.hpp"

namespace cgp
{

	struct camera_first_person_euler : camera_generic_base
	{
		// Camera parameters
		//***********************************************// 

		float roll;  // Twist around itself (not used in standard orbit mode)
		float pitch; // Rotation up/down
		float yaw;   // Rotation left/right

		// This axis is the up-direction when pitch=0
		//  Change this axis in case you want to rotate around a plane with different orientation
		vec3 axis_of_rotation;

		// The center of the camera frame
		vec3 position_camera;



		// Camera Frame Manipulator and Modifiers
		//***********************************************// 

		// Rotates the axis_of_rotation
		//  The rotation is performed around the z-direction in the default frame when pitch=0
		void manipulator_twist_rotation_axis(float magnitude);

		void manipulator_rotate_roll_pitch_yaw(float d_roll, float d_pitch, float d_yaw);

		// Translate the camera in the (right,up) plane
		void manipulator_translate_in_plane(vec2 const& tr);
		// Translate the camera in the front direction
		void manipulator_translate_front(float magnitude);

		// Change the rotation axis
		void set_rotation_axis(vec3 axis);

		void look_at(vec3 const& eye, vec3 const& center);

		camera_first_person_euler();


		// Camera Frame Computation
		//***********************************************// 

		// The position of the camera frame
		vec3 position() const;
		// The orientation of the camera frame
		rotation_transform orientation() const;
	};
}