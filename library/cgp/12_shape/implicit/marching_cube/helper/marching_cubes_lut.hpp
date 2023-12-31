#pragma once

#include "cgp/02_numarray/numarray_stack/numarray_stack.hpp"
#include <array>

namespace cgp {

	// Local offset to apply to get the 8 vertex of the cube
	std::array<int3, 8> marching_cube_lut_offset_cube();
	// Order of edges on the cube
	std::array<std::pair<int, int>, 12> marching_cube_lut_edge_order();

	std::array<int, 256> marching_cube_lut_edgeTable();
	std::array<std::array<int, 16>, 256> marching_cube_lut_triTable();
}