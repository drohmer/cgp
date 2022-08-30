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

	// Copmpute inverse of mat (using determinants/Cramer rule)
	mat2 inverse(mat2 const& m);
	mat3 inverse(mat3 const& m);
	mat4 inverse(mat4 const& m);

	// Transformation in homogeneous coordinates applied to a vec3 using a mat4 (special case of mat4 and vec3)
	//  Assume p=(x,y,z,1); 
	//  Compute p' = M*p, and returns the normalized vector (x'/w', y'/w', z'/w')
	vec3 operator*(matrix_stack<float, 4, 4> const& M, vec3 const& p);
}

