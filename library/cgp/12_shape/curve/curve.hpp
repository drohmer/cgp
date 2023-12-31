#pragma once

#include "cgp/02_numarray/numarray.hpp"

namespace cgp
{
	numarray<vec3> curve_primitive_circle(float radius=1.0f, vec3 const& center={0,0,0}, vec3 const& normal={0,0,1}, int N_sample=20);

	/* Convert a series of successive points (p0,p1,p2, ..., pn) into segments by duplicating points (p0,p1, p1,p2, ..., pn-1,pn) */
	numarray<vec3> curve_to_segments(numarray<vec3> const& curve_in);
}