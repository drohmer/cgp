#include "cgp/05_vec/vec.hpp"
#include "cgp/06_mat/mat.hpp"

#if defined(__linux__) || defined(__EMSCRIPTEN__)
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

#include <iostream>

namespace cgp_test 
{

	void test_matrix_stack()
	{
		// element access
		{
			using namespace cgp;
			matrix_stack<int, 2, 3> a = { 1,2,3, 4,5,6 };
			
			assert_cgp_no_msg(a(0, 0) == 1); assert_cgp_no_msg(a(0, 1) == 2); assert_cgp_no_msg(a(0, 2) == 3);
			assert_cgp_no_msg(a(1, 0) == 4); assert_cgp_no_msg(a(1, 1) == 5); assert_cgp_no_msg(a(1, 2) == 6);

			assert_cgp_no_msg(a(0)(0) == 1); assert_cgp_no_msg(a(0)(1) == 2); assert_cgp_no_msg(a(0)(2) == 3);
			assert_cgp_no_msg(a(1)(0) == 4); assert_cgp_no_msg(a(1)(1) == 5); assert_cgp_no_msg(a(1)(2) == 6);

			assert_cgp_no_msg(a[0][0] == 1); assert_cgp_no_msg(a[0][1] == 2); assert_cgp_no_msg(a[0][2] == 3);
			assert_cgp_no_msg(a[1][0] == 4); assert_cgp_no_msg(a[1][1] == 5); assert_cgp_no_msg(a[1][2] == 6);


			assert_cgp_no_msg( is_equal(a[0], numarray_stack<int, 3>{ 1,2,3 }) && is_equal(a[1], numarray_stack<int, 3>{ 4,5,6 }) );
			assert_cgp_no_msg( is_equal(a(0), numarray_stack<int, 3>{ 1, 2, 3 }) && is_equal(a(1), numarray_stack<int, 3>{ 4, 5, 6 }));


			assert_cgp_no_msg( is_equal(get<0, 0>(a), 1) ); assert_cgp_no_msg(is_equal(get<0, 1>(a), 2)); assert_cgp_no_msg(is_equal(get<0, 2>(a), 3));
			assert_cgp_no_msg( is_equal(get<1, 0>(a), 4)); assert_cgp_no_msg(is_equal(get<1, 1>(a), 5)); assert_cgp_no_msg(is_equal(get<1, 2>(a), 6));

			assert_cgp_no_msg(is_equal(get_offset<0>(a), 1)); assert_cgp_no_msg(is_equal(get_offset<1>(a), 2)); assert_cgp_no_msg(is_equal(get_offset<2>(a), 3));
			assert_cgp_no_msg(is_equal(get_offset<3>(a), 4)); assert_cgp_no_msg(is_equal(get_offset<4>(a), 5)); assert_cgp_no_msg(is_equal(get_offset<5>(a), 6));

			assert_cgp_no_msg(is_equal(get<0>(a), numarray_stack<int, 3>{ 1,2,3 }) && is_equal(get<1>(a), numarray_stack<int, 3>{ 4,5,6 }) );

			assert_cgp_no_msg(type_str(a) == "matrix_stack<int,2,3>");
		}

		// matrix multiplication
		{
			cgp::matrix_stack<int, 2, 3> a = { 1,2,3, 4,5,6 };
			cgp::matrix_stack<int, 3, 2> b = { 5,-1, 4,8, -2,2 };

			cgp::matrix_stack<int, 2, 2> c = a * b;
			assert_cgp_no_msg(is_equal(c, cgp::matrix_stack<int, 2,2>{7,21,28,48}));
		}


		// matrix componentwise multiplication
		{
			cgp::matrix_stack<int, 2, 3> a = { 1,2,3, 4,5,6 };
			cgp::matrix_stack<int, 2, 3> b = { 5,2,-1, 2,1,-2 };

			cgp::matrix_stack<int, 2, 3> c = cgp::multiply_componentwise(a, b);
			assert_cgp_no_msg(is_equal(c, cgp::matrix_stack<int, 2, 3>{5,4,-3, 8,5,-12}));
		}

		// matrix vector
		{
			cgp::matrix_stack<int, 2, 3> a = { 1,2,3, 4,5,6 };
			cgp::numarray_stack<int, 3> b = { 5,-2,3 };
			cgp::numarray_stack<int, 2> c = a * b;
			assert_cgp_no_msg(is_equal(c, cgp::numarray_stack<int, 2>{10, 28}));
		}


		// remove column row
		{
			cgp::matrix_stack<int, 5, 4> a = { 1,2,3,4, 5,6,7,8, 9,10,11,12, 13,14,15,16, 17,18,19,20};

			assert_cgp_no_msg( is_equal(a.remove_row_column(1, 2), cgp::matrix_stack<int,4,3>{1,2,4, 9,10,12, 13,14,16, 17,18,20} ) );
			assert_cgp_no_msg( is_equal(a.remove_row_column(0, 0), cgp::matrix_stack<int,4,3>{6,7,8, 10,11,12, 14,15,16, 18,19,20} ) );
			assert_cgp_no_msg( is_equal(a.remove_row_column(4, 3), cgp::matrix_stack<int,4,3>{1,2,3, 5,6,7, 9,10,11, 13,14,15} ) );


		}

		// identity
		{
			assert_cgp_no_msg(is_equal(cgp::matrix_stack<float, 3, 3>::build_identity(), cgp::matrix_stack<float, 3, 3>{ 1,0,0, 0,1,0, 0,0,1 }));
			assert_cgp_no_msg(is_equal(cgp::matrix_stack<float, 3, 4>::build_identity(), cgp::matrix_stack<float, 3, 4>{ 1,0,0,0, 0,1,0,0, 0,0,1,0 }));
			assert_cgp_no_msg(is_equal(cgp::matrix_stack<float, 4, 3>::build_identity(), cgp::matrix_stack<float, 4, 3>{ 1,0,0, 0,1,0, 0,0,1, 0,0,0 }));
		}

		// construct from different size
		{
			using namespace cgp;
			{
				mat3 M1 = { 1,2,3,
							4,5,6,
							7,8,9 };
				mat2 M2 = mat2(M1);
				mat4 M4 = mat4(M1);
				assert_cgp_no_msg(is_equal(M2, mat2{ 1,2,4,5 }));
				assert_cgp_no_msg(is_equal(M4, mat4{ 1,2,3,0, 4,5,6,0, 7,8,9,0, 0,0,0,1 }));
			}
			{
				mat4 M1 = { 1 , 2, 3, 4,
							5 , 6, 7, 8,
							9 ,10,11,12,
							13,14,15,16
				};
				mat2 M2 = mat2(M1);
				mat3 M3 = mat3(M1);
				assert_cgp_no_msg(is_equal(M2, mat2{ 1,2,5,6 }));
				assert_cgp_no_msg(is_equal(M3, mat3{ 1,2,3, 5,6,7, 9,10,11 }));
			}
		}

		// set block
		{
			using namespace cgp;

			assert_cgp_no_msg( is_equal(mat4().set_block(mat2{ 1,2,3,4 }), mat4{ 1,2,0,0, 3,4,0,0, 0,0,0,0, 0,0,0,0 }));
			assert_cgp_no_msg( is_equal(mat4().set_block(mat2{ 1,2,3,4 }, 0, 1), mat4{ 0,1,2,0, 0,3,4,0, 0,0,0,0, 0,0,0,0 }));
			assert_cgp_no_msg( is_equal(mat4().set_block(mat2{ 1,2,3,4 }, 1, 0), mat4{ 0,0,0,0, 1,2,0,0, 3,4,0,0, 0,0,0,0 }));
		}


		// mat * vec
		{
			using namespace cgp;

			{
				mat2 const M = { 1,2, 3,4 };
				vec2 const x = { 4,2 };
				assert_cgp_no_msg(is_equal(M * x, vec2{ 8,20 }));
			}

			{
				mat3 const M = { 1,2,1, 3,4,5, 1,2,7 };
				vec3 const x = { 4,2,6 };
				assert_cgp_no_msg(is_equal(M * x, vec3{ 14,50,50 }));
			}

			{
				mat4 const M = { 1,2,1,5, 3,4,5,-2, 1,2,7,3, 5,4,7,-1 };
				vec4 const x = { 4,2,6,3 };
				assert_cgp_no_msg(is_equal(M * x, vec4{ 29,44,59,67 }));
			}
		}

		// Operators
		{
			using namespace cgp;
			{
				cgp::matrix_stack<int, 3, 2> A = {2,4, 5,1, 0,1};
				cgp::matrix_stack<int, 3, 2> B = {4,1, 1,2, 3,2};

				A -= B;
				assert_cgp_no_msg(is_equal(A, cgp::matrix_stack<int, 3, 2>{-2,3, 4,-1, -3,-1}));
				A += B;
				assert_cgp_no_msg(is_equal(A, cgp::matrix_stack<int, 3, 2>{2, 4, 5, 1, 0, 1}));
			}

			{
				cgp::matrix_stack<int, 2, 3> A = { 2,4,5, 1,0,1 };
				int b=2;
				A *= b;
				assert_cgp_no_msg(is_equal(A, cgp::matrix_stack<int, 2, 3>{4,8,10, 2,0,2}));
			}

			{
				cgp::matrix_stack<float, 2, 3> A = { 2,4,5, 1,0,1 };
				float b = 2;
				A /= b;
				assert_cgp_no_msg(is_equal(A, cgp::matrix_stack<float, 2, 3>{1,2, 2.5,0.5, 0,0.5}));
			}

		}


		{
			using namespace cgp;
			{
				mat2 a = { 1,2, 3,4 };
				a *= 2.0f;
				assert_cgp_no_msg(is_equal(a, mat2{ 2,4, 6,8 }));
			}
			{
				mat2 const a = { 1,2, 3,4 };
				mat2 b = -a;
				assert_cgp_no_msg(is_equal(b, mat2{ -1,-2, -3,-4 }));
			}

		}

		{
			using namespace cgp;
			{
				matrix_stack<float, 3, 1> M = vec3(1, 2, 3);
				assert_cgp_no_msg(is_equal(M, matrix_stack<float,3, 1>{ 1,2,3 }));
			}
			{
				matrix_stack<float, 1, 3> M = vec3(1, 2, 3);
				assert_cgp_no_msg(is_equal(M, matrix_stack<float, 1, 3>{ 1, 2, 3 }));
			}
		}

		{
			using namespace cgp;
			{
				mat2 M2; M2.fill(2.0f);
				mat3 M3; M3.fill(2.0f);
				mat4 M4; M4.fill(2.0f);
				matrix_stack<float, 2, 3> M; M.fill(2.0f);

				assert_cgp_no_msg(is_equal(M2, mat2{ 2, 2, 2,2 }));
				assert_cgp_no_msg(is_equal(M3, mat3{ 2,2,2, 2,2,2, 2,2,2 }));
				assert_cgp_no_msg(is_equal(M4, mat4{ 2,2,2,2, 2,2,2,2, 2,2,2,2, 2,2,2,2 }));
				assert_cgp_no_msg(is_equal(M, matrix_stack<float, 2, 3>{ 2,2,2, 2,2,2 }));
			}
		}

		{
			using namespace cgp;
			{
				mat4 M = mat4::build_scaling(5, 3, 2).set_block_translation(1, 2, 3);
				vec3 t = { 4,8,-1 };
				vec3 res = M.transform_position(t);
				assert_cgp_no_msg(is_equal(res, { 21,26,1 }));
			}
			{
				mat4 M = mat4::build_scaling(5, 3, 2).set_block_translation(1, 2, 3);
				vec3 t = { 4,8,-1 };
				vec3 res = M.transform_vector(t);
				assert_cgp_no_msg(is_equal(res, { 20,24,-2 }));
			}
			{
				mat4 M = mat4::build_scaling(5, 3, 2).set_block_translation(1, 2, 3);
				vec3 t = { 4,8,-1 };
				vec4 res_tmp = M * vec4(t,1.0f);
				vec3 res = res_tmp.xyz() / res_tmp.w;
				assert_cgp_no_msg(is_equal(res, { 21,26,1 }));
			}
			{
				mat4 M = mat4::build_scaling(5, 3, 2).set_block_translation(1, 2, 3);
				vec3 t = { 4,8,-1 };
				vec3 res = (M * vec4(t,0.0)).xyz();
				assert_cgp_no_msg(is_equal(res, { 20,24,-2 }));
			}

			{
				mat4 M = mat4::build_scaling(5, 3, 2).set_block_translation(1, 2, 3);
				M(3, 3) = 2;
				vec3 t = { 4,8,-1 };
				vec3 res = M.transform_position(t);
				assert_cgp_no_msg(is_equal(res, { 21/2.0f,26/2.0f,1/2.0f }));
			}
			{
				mat4 M = mat4(
					1, 5, 8, 2, 
					2, 1, -2, 3, 
					5, 2, 8, 4, 
					0.5f, -1, 2, 2);
				vec3 t = { 4,8,-1 };
				vec3 res = M .transform_position(t);
				assert_cgp_no_msg(is_equal(res, { -38/6.0f, -21/6.0f, -32/6.0f }));
			}
		}

		// test initialization
		{
			using namespace cgp;

			mat4 M1(1.0f);
			mat4 M2(2.0f);
			mat4 M3(2.0f, 3.0f, 7.0f);
			mat4 M4(2.0f, 3.0f, 7.0f, 2.0f);

			assert_cgp_no_msg(is_equal(M1, mat4{ 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 }));
			assert_cgp_no_msg(is_equal(M2, mat4{ 2,0,0,0, 0,2,0,0, 0,0,2,0, 0,0,0,2 }));
			assert_cgp_no_msg(is_equal(M3, mat4{ 2,0,0,0, 0,3,0,0, 0,0,7,0, 0,0,0,1 }));
			assert_cgp_no_msg(is_equal(M4, mat4{ 2,0,0,0, 0,3,0,0, 0,0,7,0, 0,0,0,2 }));
		}
		// test initialization with initializer-list
		{
			using namespace cgp;

			mat4 M1 = {1.0f};
			mat4 M2 = {2.0f};
			mat4 M3 = {2.0f, 3.0f, 7.0f};
			mat4 M4 = {2.0f, 3.0f, 7.0f, 2.0f};

			assert_cgp_no_msg(is_equal(M1, mat4{ 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 }));
			assert_cgp_no_msg(is_equal(M2, mat4{ 2,0,0,0, 0,2,0,0, 0,0,2,0, 0,0,0,2 }));
			assert_cgp_no_msg(is_equal(M3, mat4{ 2,0,0,0, 0,3,0,0, 0,0,7,0, 0,0,0,1 }));
			assert_cgp_no_msg(is_equal(M4, mat4{ 2,0,0,0, 0,3,0,0, 0,0,7,0, 0,0,0,2 }));
		}
		{
			using namespace cgp;

			mat3 M1(1.0f);
			mat3 M2(2.0f);
			mat3 M3(2.0f, 3.0f, 7.0f);
			mat3 M1b = {1.0f};
			mat3 M2b = {2.0f};
			mat3 M3b = {2.0f, 3.0f, 7.0f};

			assert_cgp_no_msg(is_equal(M1, mat3{ 1,0,0, 0,1,0, 0,0,1 }));
			assert_cgp_no_msg(is_equal(M2, mat3{ 2,0,0, 0,2,0, 0,0,2 }));
			assert_cgp_no_msg(is_equal(M3, mat3{ 2,0,0, 0,3,0, 0,0,7 }));

			assert_cgp_no_msg(is_equal(M1b, mat3{ 1,0,0, 0,1,0, 0,0,1 }));
			assert_cgp_no_msg(is_equal(M2b, mat3{ 2,0,0, 0,2,0, 0,0,2 }));
			assert_cgp_no_msg(is_equal(M3b, mat3{ 2,0,0, 0,3,0, 0,0,7 }));
		}
		{
			using namespace cgp;

			mat2 M1(1.0f);
			mat2 M2(2.0f);
			mat2 M3(2.0f, 3.0f);
			mat2 M1b = {1.0f};
			mat2 M2b = {2.0f};
			mat2 M3b = {2.0f, 3.0f};

			assert_cgp_no_msg(is_equal(M1, mat2{ 1,0, 0,1 }));
			assert_cgp_no_msg(is_equal(M2, mat2{ 2,0, 0,2 }));
			assert_cgp_no_msg(is_equal(M3, mat2{ 2,0, 0,3 }));

			assert_cgp_no_msg(is_equal(M1b, mat2{ 1,0, 0,1 }));
			assert_cgp_no_msg(is_equal(M2b, mat2{ 2,0, 0,2 }));
			assert_cgp_no_msg(is_equal(M3b, mat2{ 2,0, 0,3 }));
		}


	}
}