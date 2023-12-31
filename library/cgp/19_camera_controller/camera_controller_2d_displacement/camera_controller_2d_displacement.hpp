#pragma once

#include "../camera_controller_first_person_euler/camera_controller_first_person_euler.hpp"

namespace cgp {

struct camera_controller_2d_displacement : camera_controller_first_person_euler
{
	void action_mouse_move(mat4& camera_matrix_view);
	void idle_frame(mat4& camera_matrix_view);

	std::string doc_usage() const;
};


}