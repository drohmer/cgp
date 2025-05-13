#pragma once

#include "cgp/cgp.hpp"

namespace cgp
{

/** A polygon with hole structure
*/
struct polygon
{
    /** collection of polygonal rings. ring[0] is the outer ring */
    //buffer<buffer<vec3>> ring;
    numarray<numarray<vec3>> ring;

};

void polygon_to_curves(numarray<curve_drawable>& curves, const polygon& poly, const vec3 color);
void polygons_to_curves(numarray<curve_drawable>& curves, const numarray<polygon>& polygons, const vec3 color);

cgp::mesh polygon_to_mesh(const polygon& poly);
cgp::mesh polygons_to_mesh(const numarray<polygon>& polygons);

}
