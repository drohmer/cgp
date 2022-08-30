#include "cgp/core/base/base.hpp"
#include "affine.hpp"

#include "../affine_rt/affine_rt.hpp"
#include "../affine_rts/affine_rts.hpp"

namespace cgp
{
	affine::affine()
		:rotation(), translation(), scaling(1.0f), scaling_xyz({ 1.0f, 1.0f, 1.0f })
	{}
	affine::affine(affine_rt const& T)
		:rotation(T.rotation), translation(T.translation), scaling(1.0f), scaling_xyz({ 1.0f, 1.0f, 1.0f })
	{}
	affine::affine(affine_rts const& T)
		: rotation(T.rotation), translation(T.translation), scaling(T.scaling), scaling_xyz({ 1.0f, 1.0f, 1.0f })
	{}
	affine::affine(rotation_transform const& rotation_arg, vec3 const& translation_arg, float scaling_arg, vec3 const& scaling_xyz)
		:rotation(rotation_arg), translation(translation_arg), scaling(scaling_arg), scaling_xyz(scaling_xyz)
	{}

	mat4 affine::matrix() const
	{
		mat3 const& R = rotation.matrix();
		float const sx = scaling_xyz.x * scaling;
		float const sy = scaling_xyz.y * scaling;
		float const sz = scaling_xyz.z * scaling;
		return mat4{ 
			sx * get<0,0>(R), sx * get<0,1>(R)  , sx * get<0,2>(R), translation.x,
			sy * get<1,0>(R), sy * get<1,1>(R)  , sy * get<1,2>(R), translation.y,
			sz * get<2,0>(R), sz * get<2,1>(R)  , sz * get<2,2>(R), translation.z,
			0.0f  ,  0.0f ,  0.0f ,    1.0f     
		};
		
	}

	vec3 operator*(affine const& T, vec3 const& p)
	{
		mat3 const R = T.rotation.matrix();
		
		return vec3{
			T.scaling_xyz.x* T.scaling* (get<0,0>(R)*p.x + get<0,1>(R)*p.y + get<0,2>(R)*p.z) + T.translation.x,
			T.scaling_xyz.y* T.scaling* (get<1,0>(R)*p.x + get<1,1>(R)*p.y + get<1,2>(R)*p.z) + T.translation.y,
			T.scaling_xyz.z* T.scaling* (get<2,0>(R)*p.x + get<2,1>(R)*p.y + get<2,2>(R)*p.z) + T.translation.z
		};
	}
	
	vec4 operator*(affine const& T, vec4 const& p)
	{
		mat3 const R = T.rotation.matrix();

		return vec4{
			T.scaling_xyz.x * T.scaling * (get<0,0>(R) * p.x + get<0,1>(R) * p.y + get<0,2>(R) * p.z) + T.translation.x,
			T.scaling_xyz.y * T.scaling * (get<1,0>(R) * p.x + get<1,1>(R) * p.y + get<1,2>(R) * p.z) + T.translation.y,
			T.scaling_xyz.z * T.scaling * (get<2,0>(R) * p.x + get<2,1>(R) * p.y + get<2,2>(R) * p.z) + T.translation.z,
			p.w
		};
	}


	affine operator*(affine const& T1, affine_rts const& T2)
	{
		return affine(T1.rotation * T2.rotation, T1 * T2.translation, T1.scaling * T2.scaling, T1.scaling_xyz);
	}
	affine operator*(affine const& T1, affine_rt const& T2)
	{
		return affine(T1.rotation * T2.rotation, T1 * T2.translation, T1.scaling, T1.scaling_xyz);
	}


	affine inverse(affine const& T)
	{
		rotation_transform const R_inv = inverse(T.rotation);
		return affine(R_inv, -(R_inv*T.translation), 1.0f/T.scaling, vec3{1.0f/T.scaling_xyz.x, 1.0f / T.scaling_xyz.y, 1.0f / T.scaling_xyz.z });
	}


	std::string type_str(affine const&)
	{
		return "affine";
	}
	std::string str(affine const& T)
	{
		return str(T.rotation)+" "+str(T.translation)+" "+str(T.scaling)+" "+str(T.scaling_xyz);
	}
	std::ostream& operator<<(std::ostream& s, affine const& T)
	{
		s << str(T);
		return s;
	}



}