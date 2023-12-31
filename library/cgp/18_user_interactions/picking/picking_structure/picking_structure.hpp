#pragma once

#include "cgp/05_vec/vec.hpp"

namespace cgp {

	struct picking_structure
	{
		picking_structure();

		bool active;         // true if a vertex has been selected
		
		int index;           // The index corresponding to the picked element
		vec3 position;       // The 3D position corresponding to the picking
		vec3 normal;         // The normal of the shape at the picked position (when picking occured)

		vec3 ray_origin;     // The origin from which the picking is thrown
		vec3 ray_direction;  // The direction of the ray used by the picking

		vec2 screen_clicked; // 2D position on screen where the mouse was clicked when the picking occured
	};

}