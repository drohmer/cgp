#include "cgp/core/base/base.hpp"
#include "mat_functions.hpp"

namespace cgp
{


	vec2 orthogonal_vector(vec2 const& v)
	{
		if(is_equal(norm(v),0.0f))
			return vec2{1,0};

		return normalize(vec2{v.y,-v.x});
	}
	vec3 orthogonal_vector(vec3 const& v)
	{
		if(is_equal(norm(v),0.0f))
			return vec3{1,0,0};

		vec3 const va = vec3{1,0,0};
		vec3 const test_a = cross(va, v);
		if( norm(test_a)>1e-5f )
			return normalize(test_a);

		vec3 const vb = vec3{0,1,0};
		vec3 const test_b = cross(vb, v);
		if( norm(test_b)>1e-5f )
			return normalize(test_b);

		vec3 const vc = vec3{0,0,1};
		vec3 const test_c = cross(vc, v);
		if( norm(test_c)>1e-5f )
			return normalize(test_c);

		error_cgp("Cannot find orthogonal vector to "+str(v));
	}


	float det(mat2 const& m)
	{
		return get<0,0>(m)*get<1,1>(m) - get<0,1>(m)*get<1,0>(m);
	}
	float det(mat3 const& m)
	{
		float const xx = get<0,0>(m);
		float const xy = get<0,1>(m);
		float const xz = get<0,2>(m);

		float const yx = get<1,0>(m);
		float const yy = get<1,1>(m);
		float const yz = get<1,2>(m);

		float const zx = get<2,0>(m);
		float const zy = get<2,1>(m);
		float const zz = get<2,2>(m);

		return xx*yy*zz + xy*yz*zx + yx*zy*xz - (zx*yy*xz + zy*yz*xx + yx*xy*zz);
	}
	float det(mat4 const& m);

	mat2 inverse(mat2 const& m)
	{
		float const d = det(m);
		assert_cgp( std::abs(d)>1e-5f , "Determinant is null");

		return mat2{get<1,1>(m), -get<0,1>(m),
			-get<1,0>(m), get<0,0>(m)}/d;
	}
	mat3 inverse(mat3 const& m)
	{
		float const d = det(m);
		assert_cgp( std::abs(d)>1e-5f , "Determinant is null");

		/** xx xy xz
		    yx yy yz
			zx zy zz 	*/
		float const xx = get<0,0>(m);
		float const xy = get<0,1>(m);
		float const xz = get<0,2>(m);

		float const yx = get<1,0>(m);
		float const yy = get<1,1>(m);
		float const yz = get<1,2>(m);

		float const zx = get<2,0>(m);
		float const zy = get<2,1>(m);
		float const zz = get<2,2>(m);

		float const x00 =   yy*zz-zy*yz;
		float const x10 = -(yx*zz-zx*yz);
		float const x20 =   yx*zy-zx*yy;

		float const x01 =  -(xy*zz-zy*xz);
		float const x11 =    xx*zz-zx*xz;
		float const x21 =  -(xx*zy-zx*xy);

		float const x02 =    xy*yz-yy*xz;
		float const x12 =  -(xx*yz-yx*xz);
		float const x22 =    xx*yy-yx*xy;

		return mat3{x00,x01,x02, x10,x11,x12, x20,x21,x22}/d;
	}


	float det(mat4 const& m)
	{
		return -m(3,0)*det(m.remove_row_column(3,0)) + m(3,1)*det(m.remove_row_column(3,1)) - m(3,2)*det(m.remove_row_column(3,2)) + m(3,3)*det(m.remove_row_column(3,3));
	}

	mat4 inverse(mat4 const& m)
	{
		float const d = det(m);
		assert_cgp( std::abs(d)>1e-5f , "Determinant is null");

		mat4 inv = mat4{
			 det(m.remove_row_column(0,0)), -det(m.remove_row_column(1,0)),  det(m.remove_row_column(2,0)), -det(m.remove_row_column(3,0)),
			-det(m.remove_row_column(0,1)),  det(m.remove_row_column(1,1)), -det(m.remove_row_column(2,1)),  det(m.remove_row_column(3,1)),
			 det(m.remove_row_column(0,2)), -det(m.remove_row_column(1,2)),  det(m.remove_row_column(2,2)), -det(m.remove_row_column(3,2)),
			-det(m.remove_row_column(0,3)),  det(m.remove_row_column(1,3)), -det(m.remove_row_column(2,3)),  det(m.remove_row_column(3,3)),
		}/d;

		return inv;


	}


	vec3 operator*(matrix_stack<float, 4, 4> const& M, vec3 const& p)
	{
		float const x = M.data.x.x * p.x + M.data.x.y * p.y + M.data.x.z * p.z + M.data.x.w;
		float const y = M.data.y.x * p.x + M.data.y.y * p.y + M.data.y.z * p.z + M.data.y.w;
		float const z = M.data.z.x * p.x + M.data.z.y * p.y + M.data.z.z * p.z + M.data.z.w;
		float const w = M.data.w.x * p.x + M.data.w.y * p.y + M.data.w.z * p.z + M.data.w.w;

		return vec3(x / w, y / w, z / w);
	}



}