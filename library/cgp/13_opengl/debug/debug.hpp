#pragma once

#include "cgp/opengl_include.hpp"
#include <string>

#ifndef CGP_NO_DEBUG
#define opengl_check {cgp::check_opengl_error(__FILE__, __func__, __LINE__);}
#else
#define opengl_check {}
#endif

namespace cgp
{
	std::string opengl_info_display();
	void check_opengl_error(const std::string& file, const std::string& function, int line);
}

