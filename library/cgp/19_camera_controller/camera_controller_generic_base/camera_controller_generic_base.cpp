
#include "camera_controller_generic_base.hpp"
#include "cgp/01_base/base.hpp"

#include <iostream>

namespace cgp
{
	void camera_controller_generic_base::initialize(input_devices& inputs_param, window_structure& window_param)
	{
		inputs = &inputs_param;
		window = &window_param;
	}


	std::string camera_controller_generic_base::doc_usage()
	{
		std::string doc = "You seem to be using a Controller Generic Base - This controller is only a generic class and should be specialized.\n";
		return doc;
	}
}