#pragma once

#include "../camera_controller_generic_base/camera_controller_generic_base.hpp"

namespace cgp
{
	// Specialized camera controller representing an "orbit" camera motion (= camera that rotates around a central point)
	// 	- camera_controller_orbit relies on an internal camera using a quaternion to describe the camera orientation.
	//  This camera controller allows to rotate the camera in arbitrary direction in 3D and models by default an ArcBall system
	struct camera_controller_orbit : camera_controller_generic_base
	{
		camera_orbit camera_model;

		void action_mouse_move(mat4& camera_matrix_view);
		void idle_frame(mat4& camera_matrix_view);

		void look_at(vec3 const& eye, vec3 const& center, vec3 const& up);
		void update(mat4& camera_matrix_view);

		std::string doc_usage() const;
	};



}