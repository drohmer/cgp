#include "environment.hpp"


namespace cgp
{
	bool environment_generic_structure::default_expected_uniform = true;
	
	void environment_generic_structure::send_opengl_uniform(opengl_shader_structure const&, bool) const
	{
	}


}