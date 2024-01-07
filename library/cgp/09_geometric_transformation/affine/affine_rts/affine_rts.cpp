#include "cgp/01_base/base.hpp"
#include "affine_rts.hpp"

#include "../affine_rt/affine_rt.hpp"

namespace cgp
{
	affine_rts::affine_rts()
		:rotation(), translation(), scaling(1.0f)
	{}
	affine_rts::affine_rts(affine_rt const& T)
		:rotation(T.rotation), translation(T.translation), scaling(1.0f)
	{}
	affine_rts::affine_rts(rotation_transform const& rotation_arg, vec3 const& translation_arg, float scaling_arg)
		:rotation(rotation_arg), translation(translation_arg), scaling(scaling_arg)
	{}

	mat4 affine_rts::matrix() const
	{
		mat3 const& R = rotation.matrix();
		return mat4{ 
			scaling * get<0,0>(R), scaling * get<0,1>(R)  , scaling * get<0,2>(R), translation.x,
			scaling * get<1,0>(R), scaling * get<1,1>(R)  , scaling * get<1,2>(R), translation.y,
			scaling * get<2,0>(R), scaling * get<2,1>(R)  , scaling * get<2,2>(R), translation.z,
			0.0f  ,  0.0f ,  0.0f ,    1.0f     
		};
		
	}

	vec3 operator*(affine_rts const& T, vec3 const& p)
	{
		mat3 const R = T.rotation.matrix();
		
		return vec3{
			T.scaling*(get<0,0>(R)*p.x + get<0,1>(R)*p.y + get<0,2>(R)*p.z) + T.translation.x,
			T.scaling*(get<1,0>(R)*p.x + get<1,1>(R)*p.y + get<1,2>(R)*p.z) + T.translation.y,
			T.scaling*(get<2,0>(R)*p.x + get<2,1>(R)*p.y + get<2,2>(R)*p.z) + T.translation.z
		};
	}
	vec4 operator*(affine_rts const& T, vec4 const& p)
	{
		mat3 const R = T.rotation.matrix();
		return vec4{
			T.scaling*(get<0,0>(R)*p.x + get<0,1>(R)*p.y + get<0,2>(R)*p.z) + T.translation.x*p.w,
			T.scaling*(get<1,0>(R)*p.x + get<1,1>(R)*p.y + get<1,2>(R)*p.z) + T.translation.y*p.w,
			T.scaling*(get<2,0>(R)*p.x + get<2,1>(R)*p.y + get<2,2>(R)*p.z) + T.translation.z*p.w,
			p.w
		};
	}

	affine_rts inverse(affine_rts const& T)
	{
		rotation_transform const R_inv = inverse(T.rotation);
		return affine_rts(R_inv, -(R_inv*T.translation), 1.0f/T.scaling);
	}

	affine_rts operator*(affine_rts const& T1, affine_rts const& T2)
	{
		return affine_rts( T1.rotation * T2.rotation, T1.scaling*(T1.rotation*T2.translation)+T1.translation, T1.scaling*T2.scaling);
	}


	affine_rts operator*(affine_rts const& T, rotation_transform const& R)
	{
		/** (T.s T.R | T.t) ( R | 0) = (T.s T.R R | T.t)
		*   (      0 |   1) ( 0 | 1)   (        0 |   1) */
		return affine_rts(T.rotation * R, T.translation, T.scaling);
	}
	affine_rts operator*(rotation_transform const& R, affine_rts const& T)
	{
		/** ( R | 0) (T.s T.R | T.t) = (T.s R T.R | R T.t)
		*   ( 0 | 1) (      0 |   1)   (        0 |     1) */
		return affine_rts(R * T.rotation, R*T.translation, T.scaling);
	}

	affine_rts operator+(vec3 const& tr, affine_rts const& T)
	{
		return affine_rts(T.rotation, T.translation+tr, T.scaling);
	}
	affine_rts operator+(affine_rts const& T, vec3 const& tr)
	{
		return affine_rts(T.rotation, T.translation+tr, T.scaling);
	}
	affine_rts operator-(affine_rts const& T, vec3 const& tr)
	{
		return affine_rts(T.rotation, T.translation-tr, T.scaling);
	}

	affine_rts operator*(float s, affine_rts const& T)
	{
		return affine_rts(T.rotation, T.translation, T.scaling*s);
	}
	affine_rts operator*(affine_rts const& T, float s)
	{
		return affine_rts(T.rotation, T.translation, T.scaling*s);
	}
	affine_rts operator/(affine_rts const& T, float s)
	{
		return affine_rts(T.rotation, T.translation, T.scaling/s);
	}

	affine_rts operator*(float s, rotation_transform const& r)
	{
		return affine_rts(r, {0,0,0}, s);
	}
	affine_rts operator*(rotation_transform const& r, float s)
	{
		return affine_rts(r, {0,0,0}, s);
	}

	std::string type_str(affine_rts const&)
	{
		return "affine_rt";
	}
	std::string str(affine_rts const& T)
	{
		return str(T.rotation)+" "+str(T.translation)+" "+str(T.scaling);
	}
	std::ostream& operator<<(std::ostream& s, affine_rts const& T)
	{
		s << str(T);
		return s;
	}


	affine_rts operator*(float s, affine_rt const& T)
	{
		return affine_rts(T.rotation, s*T.translation, s);
	}
	affine_rts operator*(affine_rt const& T, float s)
	{
		return affine_rts(T.rotation, s*T.translation, s);
	}


	affine_rts operator*(affine_rts const& T1, affine_rt const& T2)
	{
		/** (s1 R1 | t1) ( R2 | t2) = (s1 R1 R2 | s1 R1 t2 + t1)
		*   (  0   | 1 ) (  0 | 1 )   (     0   |     1        ) */
		return affine_rts(T1.rotation*T2.rotation, T1.scaling * T1.rotation * T2.translation + T1.translation, T1.scaling);
	}
	affine_rts operator*(affine_rt const& T1, affine_rts const& T2)
	{
		/** (  R1 | t1) ( s2 R2 | t2) = (s2 R1 R2 | R1 t2 + t1)
		*   (  0  | 1 ) (    0  | 1 )   (     0   |    1      ) */
		return affine_rts(T1.rotation * T2.rotation, T1.rotation * T2.translation + T1.translation, T2.scaling);
	}

	mat4 operator*(affine_rts const& T1, mat4 const& T2) {
		return T1.matrix()*T2;
	}
	mat4 operator*(mat4 const& T1, affine_rts const& T2) {
		return T1*T2.matrix();
	}
	mat4 operator*(affine_rts const& T1, mat3 const& T2) {
		return T1.matrix()*mat4(T2);
	}
	mat4 operator*(mat3 const& T1, affine_rts const& T2) {
		return mat4(T1)*T2.matrix();
	}

	affine_rts& affine_rts::set_scaling(float scaling_param) {
		scaling = scaling_param;
		return *this;
	}
	affine_rts& affine_rts::set_translation(vec3 const& translation_param) {
		translation = translation_param;
		return *this;
	}
	affine_rts& affine_rts::set_rotation(rotation_transform const& rotation_param) {
		rotation = rotation_param;
		return *this;
	}

}