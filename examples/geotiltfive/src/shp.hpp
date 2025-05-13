#pragma once

#include "polygon.hpp"

namespace cgp
{

	numarray<polygon> polygon_load_file_shp(const std::string& filename);
	numarray<vec3> points_load_file_shp(const std::string& filename);

}
