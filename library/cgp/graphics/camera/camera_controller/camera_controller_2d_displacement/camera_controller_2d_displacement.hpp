#pragma once

#include "../camera_controller_first_person_euler/camera_controller_first_person_euler.hpp"

namespace cgp {

struct camera_controller_2d_displacement : camera_controller_first_person_euler
{
	void idle_frame(mat4& camera_matrix_view);

};


}