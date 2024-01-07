#pragma once


#include "cgp/05_vec/vec.hpp"
#include "cgp/09_geometric_transformation/geometric_transformation.hpp"



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

		/** Apply a translation to position. Shorthand for(vec3& p: position) { p += t; } */
		mesh& translate(vec3 const& t);
		mesh& translate(float tx, float ty, float tz);
		/** Apply a scaling to position. Shorthand for(vec3& p: position) { p *= s; } */
		mesh& scale(float s);
		/** Apply non homogenous scaling to position. Also update the normals. */
		mesh& scale(float sx,float sy, float sz);
		/** Apply a rotation to position. Shorthand for(vec3& p: position) { p = R*p; }, with R the corresponding rotation. Also update the normals.*/
		mesh& rotate(vec3 const& axis, float angle);
		
		/** Apply 3x3 matrix transformation to position. Also update the normals. */
		mesh& apply_transform(mat3 const& M);
		/** Apply 4x4 matrix transformation to position. Also update the normals. */
		mesh& apply_transform(mat4 const& M);
		mesh& apply_transform(cgp::affine const& M);
		mesh& apply_transform(cgp::affine_rt const& M);
		mesh& apply_transform(cgp::affine_rts const& M);

		/** Center the mesh positions around 0*/
		mesh& centered();
		/** Normalize the bounding box of the mesh element to fit in a unit cube */
		mesh& normalize_size_to_position();

		/** Get the information from axis aligned bounding box. p_min and p_max are the two extreme corners. */
		void get_bounding_box_position(vec3& p_min, vec3& p_max) const;
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
