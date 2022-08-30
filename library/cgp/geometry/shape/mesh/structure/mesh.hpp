#pragma once


#include "cgp/core/containers/containers.hpp"
#include "cgp/geometry/vec/vec.hpp"

namespace cgp
{

	/** Standard triangular mesh structure storing per-vertex information as well as triangle connectivity
	* All data are stored contiguously in the CPU memory	 (as numarray) */
	struct mesh
	{
		numarray<vec3> position;
		numarray<vec3> normal;
		numarray<vec3> color;
		numarray<vec2> uv;
		numarray<uint3> connectivity;

		/** Fill all per-vertex attributes with default values if they are empty (ex. color to white, and 0 for texture-uv)
		* This function should be called before creating a mesh_drawable if there is empty buffers */
		mesh& fill_empty_field();

		/** Concatenate the content of another mesh to the current one */
		mesh& push_back(mesh const& to_add);
		mesh& flip_connectivity();
		mesh& normal_update();
	};

	/** Compute automaticaly a per-vertex normal given a set of positions and their connectivity 
	* Version where the normal is passed as in/out argument (usefull in case of real-time update of the normals) 
	*   allows to save time and avoid unecessary allocation if the normal vector has already the correct size.	*/
	void normal_per_vertex(numarray<vec3> const& position, numarray<uint3> const& connectivity, numarray<vec3>& normals_to_fill, bool invert=false);
	/** Compute automaticaly a per-vertex normal given a set of positions and their connectivity */
	numarray<vec3> normal_per_vertex(numarray<vec3> const& position, numarray<uint3> const& connectivity, bool invert=false);

	/** Check if the mesh looks coherent (correct indexing and size of buffer, no degenerate triangle, etc) */
	bool mesh_check(mesh const& m);


	numarray<numarray<int> > connectivity_one_ring(numarray<uint3> const& connectivity);

	std::string str(mesh const& m);
	std::string type_str(mesh const&);
}