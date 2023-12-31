#pragma once

#include "../mat2/mat2.hpp"
#include "../mat3/mat3.hpp"
#include "../mat4/mat4.hpp"

namespace cgp
{

	// Compute a unit vector orthogonal to the current one
	//  return vector v_ortho such that dot(v,v_ortho)=0
    vec2 orthogonal_vector(vec2 const& v);
	vec3 orthogonal_vector(vec3 const& v);

	// Determinant of mat
	float det(mat2 const& m);
	float det(mat3 const& m);
	float det(mat4 const& m);

	// Compute inverse of mat (using determinants/Cramer rule)
	mat2 inverse(mat2 const& m);
	mat3 inverse(mat3 const& m);
	mat4 inverse(mat4 const& m);

	// Compute the matrix resulting from a * transpose(b)
	mat2 tensor_product(vec2 const& a, vec2 const& b);
	mat3 tensor_product(vec3 const& a, vec3 const& b);
	mat4 tensor_product(vec4 const& a, vec4 const& b);

}

