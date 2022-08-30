
#include "camera_controller_generic_base.hpp"
#include "cgp/core/base/base.hpp"

#include <iostream>

namespace cgp
{
	void camera_controller_generic_base::initialize(input_devices& inputs_param, window_structure& window_param)
	{
		inputs = &inputs_param;
		window = &window_param;
	}


}