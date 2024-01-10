#pragma once

#include "cgp/09_geometric_transformation/rotation_transform/rotation_transform.hpp"
#include "cgp/05_vec/vec.hpp"
#include "cgp/06_mat/mat.hpp"

namespace cgp
{
	// ( rotation | translation )
	// (    0     |      1      )
	struct affine_rt
	{
		rotation_transform rotation;
		vec3 translation;

		affine_rt();
		explicit affine_rt(rotation_transform const& rotation);
		explicit affine_rt(rotation_transform const& rotation, vec3 const& translation);

		mat4 matrix() const;

		static affine_rt from_matrix(mat4 const& M);

		affine_rt& set_translation(vec3 const& xyz);
		affine_rt& set_rotation(rotation_transform const& r);

	};

	vec3 operator*(affine_rt const& T, vec3 const& p);
	vec4 operator*(affine_rt const& T, vec4 const& p);
	affine_rt operator*(affine_rt const& T1, affine_rt const& T2);
	
	affine_rt operator*(affine_rt const& T, rotation_transform const& R);
	affine_rt operator*(rotation_transform const& R, affine_rt const& T);

	affine_rt operator+(vec3 const& tr, affine_rt const& T);
	affine_rt operator+(affine_rt const& T, vec3 const& tr);
	affine_rt operator-(affine_rt const& T, vec3 const& tr);


	mat4 operator*(affine_rt const& T1, mat4 const& T2);
	mat4 operator*(mat4 const& T1, affine_rt const& T2);
	mat4 operator*(affine_rt const& T1, mat3 const& T2);
	mat4 operator*(mat3 const& T1, affine_rt const& T2);


	affine_rt inverse(affine_rt const& T);

	/** Transformation representing a rotation around a given center point.
	*   Transformation matrix corresponds to: 
	*     R (p-center)+center  <=> ( R | -R center + center)
	*                              ( 0 |            1      )
	*/
	affine_rt rotation_around_center(rotation_transform const& R, vec3 const& center);
	
	std::string type_str(affine_rt const& );
	std::string str(affine_rt const& T);
	std::ostream& operator<<(std::ostream& s, affine_rt const& T);
}