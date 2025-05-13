#include "polygon.hpp"

#include <limits>
#include "third_party/src/earcut/earcut.hpp"

namespace cgp
{

void polygon_to_curves(numarray<curve_drawable>& curves, const polygon& poly, const vec3 color)
{
	for(const numarray<vec3>& ring : poly.ring)
	{
		curve_drawable visual;
		visual.initialize_data_on_gpu(ring);
		visual.color = color;
		curves.push_back(visual);
	}
}

mesh polygon_to_mesh(const polygon& poly)
{
	mesh m;
	for(const numarray<vec3>& ring : poly.ring)
	{
		m.position.push_back(ring);
	}
	
	unsigned int nv = 0;
	for(unsigned int i=0; i<poly.ring.size(); ++i) 
	{
		std::vector<std::vector<vec3>> ring = {poly.ring[i].data};
		std::vector<unsigned int> indices = mapbox::earcut<unsigned int>(ring);
		for(unsigned int j = 0; j < indices.size(); j+=3)
		{
			m.connectivity.push_back({nv+indices[j], nv+indices[j+1], nv+indices[j+2]});
		}
		nv += (unsigned int)(poly.ring[i].size());
	}
	m.fill_empty_field();
	return m;
}

void polygons_to_curves(numarray<curve_drawable>& curves, const numarray<polygon>& polygons, const vec3 color)
{
	for(const polygon& poly : polygons)
	{
		polygon_to_curves(curves, poly, color);
	}
}

mesh polygons_to_mesh(const numarray<polygon>& polygons)
{
	mesh m;
	for(const polygon& poly : polygons)
	{
		m.push_back(polygon_to_mesh(poly));
	}
	return m;
}

}
