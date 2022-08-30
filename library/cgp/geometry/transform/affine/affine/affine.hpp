#pragma once

#include "cgp/geometry/transform/rotation_transform/rotation_transform.hpp"

namespace cgp
{
	struct affine_rts;
	struct affine_rt;

	// ( scaling * scaling_xyz * rotation | translation )
	// (         0                        |      1      )
	struct affine
	{
		rotation_transform rotation;
		vec3 translation;
		float scaling;
		vec3 scaling_xyz;

		affine();
		affine(affine_rt const& T);
		affine(affine_rts const& T);
		explicit affine(rotation_transform const& rotation, vec3 const& translation=vec3{0,0,0}, float scaling = 1.0f, vec3 const& scaling_xyz = vec3{1,1,1});

		mat4 matrix() const;
	};

	vec3 operator*(affine const& T, vec3 const& p);
	vec4 operator*(affine const& T, vec4 const& p);

	affine operator*(affine const& T1, affine_rts const& T2);
	affine operator*(affine const& T1, affine_rt const& T2);


	affine inverse(affine const& T);

	std::string type_str(affine const& );
	std::string str(affine const& T);
	std::ostream& operator<<(std::ostream& s, affine const& T);

}