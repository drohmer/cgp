#include "cgp/core/base/base.hpp"
#include "affine_rt.hpp"


namespace cgp
{
	affine_rt::affine_rt()
		:rotation(),translation()
	{}
	affine_rt::affine_rt(rotation_transform const& rotation_arg)
		:rotation(rotation_arg), translation()
	{}
	affine_rt::affine_rt(rotation_transform const& rotation_arg, vec3 const& translation_arg)
		:rotation(rotation_arg), translation(translation_arg)
	{}


	mat4 affine_rt::matrix() const
	{
		return mat4::build_affine(rotation.matrix(), translation);
	}

	vec3 operator*(affine_rt const& T, vec3 const& p)
	{
		mat3 const R = T.rotation.matrix();
		
		return vec3{get<0,0>(R)*p.x + get<0,1>(R)*p.y + get<0,2>(R)*p.z + T.translation.x,
			get<1,0>(R)*p.x + get<1,1>(R)*p.y + get<1,2>(R)*p.z + T.translation.y,
			get<2,0>(R)*p.x + get<2,1>(R)*p.y + get<2,2>(R)*p.z + T.translation.z
		};
	}
	vec4 operator*(affine_rt const& T, vec4 const& p)
	{
		mat3 const R = T.rotation.matrix();
		return vec4{get<0,0>(R)*p.x + get<0,1>(R)*p.y + get<0,2>(R)*p.z + T.translation.x*p.w,
			get<1,0>(R)*p.x + get<1,1>(R)*p.y + get<1,2>(R)*p.z + T.translation.y*p.w,
			get<2,0>(R)*p.x + get<2,1>(R)*p.y + get<2,2>(R)*p.z + T.translation.z*p.w,
			p.w
		};
	}

	affine_rt inverse(affine_rt const& T)
	{
		rotation_transform const R_inv = inverse(T.rotation);
		return affine_rt(R_inv, -(R_inv*T.translation));
	}

	affine_rt operator*(affine_rt const& T1, affine_rt const& T2)
	{
		return affine_rt(T1.rotation * T2.rotation, T1.rotation*T2.translation+T1.translation);
	}


	affine_rt operator*(affine_rt const& T, rotation_transform const& R)
	{
		/** ( T.R | T.t) ( R | 0) = ( T.R R | T.t)
		*   (   0 |   1) ( 0 | 1)   (     0 |   1) */
		return affine_rt(T.rotation * R, T.translation);
	}
	affine_rt operator*(rotation_transform const& R, affine_rt const& T)
	{
		/** ( R | 0) ( T.R | T.t) = ( R T.R | R T.t)
		*   ( 0 | 1) (   0 |   1)   (     0 |     1) */
		return affine_rt(R * T.rotation, R*T.translation);
	}

	affine_rt operator+(vec3 const& tr, affine_rt const& T)
	{
		return affine_rt(T.rotation, T.translation+tr);
	}
	affine_rt operator+(affine_rt const& T, vec3 const& tr)
	{
		return affine_rt(T.rotation, T.translation+tr);
	}
	affine_rt operator-(affine_rt const& T, vec3 const& tr)
	{
		return affine_rt( T.rotation, T.translation-tr);
	}

	affine_rt rotation_around_center(rotation_transform const& R, vec3 const& center)
	{
		return affine_rt{ R, -(R*center)+center };
	}


	std::string type_str(affine_rt const&)
	{
		return "affine_rt";
	}
	std::string str(affine_rt const& T)
	{
		return str(T.rotation)+" "+str(T.translation);
	}
	std::ostream& operator<<(std::ostream& s, affine_rt const& T)
	{
		s << str(T);
		return s;
	}

}