#pragma once

#include "cgp/09_geometric_transformation/rotation_transform/rotation_transform.hpp"

namespace cgp
{
	struct affine_rt;

	// ( scaling * rotation | translation )
	// (         0          |      1      )
	struct affine_rts
	{
		rotation_transform rotation;
		vec3 translation;
		float scaling;

		affine_rts();
		affine_rts(affine_rt const& T);
		explicit affine_rts(rotation_transform const& rotation, vec3 const& translation, float scaling);

		affine_rts& set_scaling(float scaling);
		affine_rts& set_translation(vec3 const& translation);
		affine_rts& set_rotation(rotation_transform const& rotation);

		mat4 matrix() const;
	};

	vec3 operator*(affine_rts const& T, vec3 const& p);
	vec4 operator*(affine_rts const& T, vec4 const& p);
	affine_rts operator*(affine_rts const& T1, affine_rts const& T2);
	
	affine_rts operator*(affine_rts const& T, rotation_transform const& R);
	affine_rts operator*(rotation_transform const& R, affine_rts const& T);

	affine_rts operator+(vec3 const& tr, affine_rts const& T);
	affine_rts operator+(affine_rts const& T, vec3 const& tr);
	affine_rts operator-(affine_rts const& T, vec3 const& tr);

	affine_rts operator*(float s, affine_rts const& T);
	affine_rts operator*(affine_rts const& T, float s);
	affine_rts operator/(affine_rts const& T, float s);


	affine_rts operator*(float s, rotation_transform const& r);
	affine_rts operator*(rotation_transform const& r, float s);

	affine_rts operator*(float s, affine_rt const& T);
	affine_rts operator*(affine_rt const& T, float s);

	affine_rts operator*(affine_rts const& T1, affine_rt const& T2);
	affine_rts operator*(affine_rt const& T1, affine_rts const& T2);

	mat4 operator*(affine_rts const& T1, mat4 const& T2);
	mat4 operator*(mat4 const& T1, affine_rts const& T2);
	mat4 operator*(affine_rts const& T1, mat3 const& T2);
	mat4 operator*(mat3 const& T1, affine_rts const& T2);


	affine_rts inverse(affine_rts const& T);

	std::string type_str(affine_rts const& );
	std::string str(affine_rts const& T);
	std::ostream& operator<<(std::ostream& s, affine_rts const& T);




	
}