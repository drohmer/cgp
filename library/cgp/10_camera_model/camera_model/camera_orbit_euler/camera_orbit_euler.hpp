#pragma once

#include "cgp/09_geometric_transformation/geometric_transformation.hpp"
#include "../camera_generic_base/camera_generic_base.hpp"

namespace cgp
{

	/* Camera orbit represented using Euler angle with convention following (roll, pitch, yaw) angles.
	* The camera rotates around the point [center_of_rotation], at a distance [distance_to_center].
	* The orientation of the camera is parameterized by the angle roll, pitch, yaw: 
	*   The initial frame is oriented toward -z, and the rotations are 
	*   - roll: rotation around (0,0,-1)
	*   - pitch: rotation around (1,0,0) 
	*   - yaw: rotation around (0,1,0)
	*   - The final rotation is R = roll * pitch * yaw, 
	* In standard use, roll is not used, therefore the camera can be seen as a frame with center given as spherical coordinates (yaw, pitch), and always target the center point
	* 
	* The orbit takes place around a specific axis defined as +y by default (ex. to rotates around a terrain given as y=f(x,z)).
	* This axis is encoded in [axis_of_rotation] and can be changed at any time.
	* 
	*/
	struct camera_orbit_euler : camera_generic_base {

		// Camera parameters
		//***********************************************// 

		float roll;  // Twist around itself (not used in standard orbit mode)
		float pitch; // Rotation up/down
		float yaw;   // Rotation left/right

		// This axis is the up-direction when pitch=0
		//  Change this axis in case you want to rotate around a plane with different orientation
		vec3 axis_of_rotation; 

		// The center point toward which the camera is looking at
		vec3 center_of_rotation;

		// The distance of the camera to the center point
		float distance_to_center;


		// Camera Frame Information
		//***********************************************// 

		// The position of the camera frame
		vec3 position() const;
		// The orientation of the camera frame
		rotation_transform orientation() const;


		// Camera Frame Manipulator and Modifiers
		//***********************************************// 

		// Apply an offset in the (rool, pitch, yaw) parameters
		//   roll += d_roll; pitch += d_pitch; yaw += d_yaw
		void manipulator_rotate_roll_pitch_yaw(float d_roll, float d_pitch, float d_yaw);

		// Rotates the axis_of_rotation
		//  The rotation is performed around the z-direction in the default frame when pitch=0
		void manipulator_twist_rotation_axis(float magnitude);

		// Increase/decrease the distance of the camera frame to the center
		//  The center remains unchanged
		void manipulator_scale_distance_to_center(float magnitude);

		// Translate the camera and the center in the view plane
		void manipulator_translate_in_plane(vec2 const& tr);
		// Translate the camera and the center orthogonally to the view plance
		void manipulator_translate_front(float magnitude);

		// Change the rotation axis
		void set_rotation_axis(vec3 axis);

		// Set the camera parameter to look at the point [center], where the camera position is [eye]
		//  Note: This function assume that roll=0, and the "up" vector is therefore constrained by the representation (it is not a degree of freedom)
		void look_at(vec3 const& eye, vec3 const& center);

		camera_orbit_euler();
	};
}