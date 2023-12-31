#pragma once

#include "cgp/04_grid_container/grid/grid.hpp"
#include "cgp/11_mesh/mesh.hpp"
#include "cgp/12_shape/spatial_domain/spatial_domain.hpp"

namespace cgp {

	/** A simple-to-use marching cube that takes as input a discrete field, a 3D domain, and the iso-value, and returns a mesh without duplicating the vertices at the same position. 
	* A new mesh is created at each call which is good for single call, but not ideal for efficiency if used in the animation loop. */
	mesh marching_cube(grid_3D<float> const& field, spatial_domain_grid_3D const& domain, float iso);


	struct marching_cube_relative_coordinates {
		size_t k0;
		size_t k1;
		float alpha;
	};

	/** A fast marching cube that generate triangles in minimizing the number of resize of not needed. The vertices of the triangles are duplicated.
	* - Return the actual number of valid vertices (that may be smaller than the size of the position)
	* - If the parameter relative is not null, it is filled with the indices of the indice grid corresponding to the edge on which the vertex lie. 
	* - Note: the parameters are set using row std::vector to handle possibly large mesh with indices using size_t instead of int */
	size_t marching_cube(std::vector<vec3>& position, std::vector<float> const& field, spatial_domain_grid_3D const& domain, float iso, std::vector<marching_cube_relative_coordinates>* relative=nullptr);
}