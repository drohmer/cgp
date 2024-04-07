#include "cgp/01_base/base.hpp"
#include "cgp/05_vec/vec.hpp"


#include <iostream>

#if defined(__linux__) || defined(__EMSCRIPTEN__)
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace cgp_test 
{

	void test_numarray_stack()
	{
		{
			cgp::numarray_stack<int,4> a = { 1,2,3,4 };
			assert_cgp_no_msg(is_equal(a, { 1,2,3,4 }));
			assert_cgp_no_msg(!is_equal(a, cgp::numarray_stack<int, 3>{ 1,2,3 }));
			assert_cgp_no_msg(!is_equal(a, cgp::numarray_stack<int, 5>{ 1, 2, 3, 4, 5 }));
		}

		{
			cgp::numarray_stack<float,3> a = { 1.1f, 2.2f, 3.1f };
			assert_cgp_no_msg(is_equal(a, { 1.1f,2.2f,3.1f }));
			assert_cgp_no_msg(!is_equal(a, cgp::numarray_stack2<float>{ 1.1f,2.2f }));
			assert_cgp_no_msg(!is_equal(a, { 1.1f,2.21f,3.1f }));
		}

		// access size 2
		{
			cgp::numarray_stack<int, 2> a = { 1,2 };
			assert_cgp_no_msg(a[0] == 1); assert_cgp_no_msg(a[1] == 2);
			assert_cgp_no_msg(a(0) == 1); assert_cgp_no_msg(a(1) == 2);
			assert_cgp_no_msg(a.at(0) == 1); assert_cgp_no_msg(a.at(1) == 2);
			assert_cgp_no_msg(cgp::get<0>(a) == 1); assert_cgp_no_msg(cgp::get<1>(a) == 2);
		}

		// access size 3
		{
			cgp::numarray_stack<int,3> a = { 1,2,3 };
			a[0] = -1;
			a[1] = 5;
			a[2] = a[1] + 1;
			assert_cgp_no_msg(is_equal(a, { -1,5,6 }));
			a = { 7,8,-2 };
			assert_cgp_no_msg(is_equal(a, { 7,8,-2 }));

			assert_cgp_no_msg(a[0] == 7); assert_cgp_no_msg(a[1] == 8); assert_cgp_no_msg(a[2] == -2);
			assert_cgp_no_msg(a(0) == 7); assert_cgp_no_msg(a(1) == 8); assert_cgp_no_msg(a(2) == -2);
			assert_cgp_no_msg(a.at(0) == 7); assert_cgp_no_msg(a.at(1) == 8); assert_cgp_no_msg(a.at(2) == -2);
			assert_cgp_no_msg(cgp::get<0>(a) == 7); assert_cgp_no_msg(cgp::get<1>(a) == 8); assert_cgp_no_msg(cgp::get<2>(a) == -2);
		}

		// access size 4
		{
			cgp::numarray_stack<int, 4> a = { 1,2,3,4 };
			assert_cgp_no_msg(a[0] == 1); assert_cgp_no_msg(a[1] == 2); assert_cgp_no_msg(a[2] == 3); assert_cgp_no_msg(a[3] == 4);
			assert_cgp_no_msg(a(0) == 1); assert_cgp_no_msg(a(1) == 2); assert_cgp_no_msg(a(2) == 3); assert_cgp_no_msg(a(3) == 4);
			assert_cgp_no_msg(a.at(0) == 1); assert_cgp_no_msg(a.at(1) == 2); assert_cgp_no_msg(a(2) == 3); assert_cgp_no_msg(a(3) == 4);
			assert_cgp_no_msg(cgp::get<0>(a) == 1); assert_cgp_no_msg(cgp::get<1>(a) == 2); assert_cgp_no_msg(cgp::get<2>(a) == 3); assert_cgp_no_msg(cgp::get<3>(a) == 4);
		}

		{
			cgp::numarray_stack<int,3> const a = { 1,2,3 };
			assert_cgp_no_msg(a[0] == 1); assert_cgp_no_msg(a(0) == 1); assert_cgp_no_msg(a.at(0) == 1);
			assert_cgp_no_msg(a[1] == 2); assert_cgp_no_msg(a(1) == 2); assert_cgp_no_msg(a.at(1) == 2);
			assert_cgp_no_msg(a[2] == 3); assert_cgp_no_msg(a(2) == 3); assert_cgp_no_msg(a.at(2) == 3);
		}

		{
			cgp::numarray_stack<int,3> a = { 1,2,3 };
			a[0] = 5; a[1] = 6; a[2] = 7;
			assert_cgp_no_msg(is_equal(a, { 5,6,7 }));
			a(0) = 2; a(1) = 1; a(2) = -7;
			assert_cgp_no_msg(is_equal(a, { 2,1,-7 }));
			a.at(0) = -1; a.at(1) = 9; a.at(2) = -7;
			assert_cgp_no_msg(is_equal(a, { -1,9,-7 }));
		}


		{
			cgp::numarray_stack<cgp::numarray_stack<int,2>,3> a = { cgp::numarray_stack<int,2>{1,2}, {2,3}, {-1,1} };
			assert_cgp_no_msg(a[0].size() == 2); assert_cgp_no_msg(is_equal(a[0], { 1,2 }));
			assert_cgp_no_msg(a[1].size() == 2); assert_cgp_no_msg(is_equal(a[1], { 2,3 }));
			assert_cgp_no_msg(a[2].size() == 2); assert_cgp_no_msg(is_equal(a[2], { -1,1 }));
		}

		{
			cgp::numarray_stack<float, 16> b; assert_cgp_no_msg(cgp::is_equal(type_str(b), "numarray_stack<float,16>"));
			cgp::numarray_stack<cgp::numarray_stack<int,5>,6> c; assert_cgp_no_msg(cgp::is_equal(type_str(c), "numarray_stack<numarray_stack<int,5>,6>"));
		}

		{
			cgp::numarray_stack<int,3> a = { 4,5,6 };
			assert_cgp_no_msg(cgp::is_equal(str(a), "4 5 6"));
			assert_cgp_no_msg(cgp::is_equal(str(a,","), "4,5,6"));
			assert_cgp_no_msg(cgp::is_equal(str(a, ",","[","]"), "[4,5,6]"));
		}

		{
			cgp::numarray_stack<int,3> a = { 4,5,6 };
			int counter = 0;
			for (int v : a) {
				if (counter == 0) assert_cgp_no_msg(v == 4);
				if (counter == 1) assert_cgp_no_msg(v == 5);
				if (counter == 2) assert_cgp_no_msg(v == 6);
				counter++;
			}
			assert_cgp_no_msg(counter==3);
		}

		{
			cgp::numarray_stack<int,3> a = { 7,5,-1 };
			assert_cgp_no_msg(cgp::size_in_memory(a)==3*sizeof(int));

			cgp::numarray_stack<cgp::numarray_stack<int,2>,3> b = { cgp::numarray_stack<int,2>{1,2},{2,3}, {-1,-1} };
			assert_cgp_no_msg(cgp::size_in_memory(b) == 6 * sizeof(int));
		}

		{
			cgp::numarray_stack<std::string, 2> a = { "hello","world" };
			assert_cgp_no_msg(cgp::is_equal(a[0], "hello"));
			assert_cgp_no_msg(cgp::is_equal(a[1], "world"));
		}

		{
			cgp::int2 a = { 8, 4 };
			assert_cgp_no_msg(a.x==8);
			assert_cgp_no_msg(a.y==4);
			assert_cgp_no_msg(type_str(a) == "int2");
		}

		{
			cgp::int3 a = { 8, 4, -1 };
			assert_cgp_no_msg(a.x == 8);
			assert_cgp_no_msg(a.y == 4);
			assert_cgp_no_msg(a.z == -1);
			assert_cgp_no_msg(type_str(a) == "int3");
		}

		{
			cgp::int4 a = { 8, 4, -1, 5 };
			assert_cgp_no_msg(a.x == 8);
			assert_cgp_no_msg(a.y == 4);
			assert_cgp_no_msg(a.z == -1);
			assert_cgp_no_msg(a.w == 5);
			assert_cgp_no_msg(type_str(a) == "int4");
		}

		{
			cgp::vec2 a = { 1.1f, 2.3f };
			cgp::vec3 b = { 1.1f, 2.3f, -1.1f };
			cgp::vec4 c = { 1.1f, 2.3f, -1.1f, 1.5f };

			assert_cgp_no_msg(type_str(a) == "vec2");
			assert_cgp_no_msg(type_str(b) == "vec3");
			assert_cgp_no_msg(type_str(c) == "vec4");
		}

		{
			cgp::vec3 a(cgp::vec2{ 1.0f,2.0f }, 3.0f);
			assert_cgp_no_msg(is_equal(a, { 1.0f,2.0f,3.0f }));
			cgp::vec3 b(1.0f, cgp::vec2{ 2.0f,3.0f });
			assert_cgp_no_msg(is_equal(b, { 1.0f,2.0f,3.0f }));
		}

		{
			using namespace cgp;
			vec3 a = {1.0f, 2, 3.0};
			vec3 b = vec3(1,2.0,3.0f);
			vec3 c = { 1.0, 2, 3.0 };
			vec3 d = { 1.0, 2.0f, 5 };


			assert_cgp_no_msg(is_equal(a, { 1.0f,2.0f,3.0f }));
			assert_cgp_no_msg(is_equal(b, { 1.0f,2.0f,3.0f }));
			assert_cgp_no_msg(is_equal(c, { 1.0f,2.0f,3.0f }));
			assert_cgp_no_msg(is_equal(d, { 1.0f,2.0f,5.0f }));
		}

		// test operators min/max/sum
		{
			using namespace cgp;
			cgp::numarray_stack<float,4> a = {4.5f, 8.2f, 6.1f, -3.6};
			assert_cgp_no_msg(cgp::is_equal(min(a), -3.6f));
			assert_cgp_no_msg(cgp::is_equal(max(a),  8.2f));
			assert_cgp_no_msg(cgp::is_equal(sum(a),  4.5f+8.2f+6.1f-3.6));
		}
		{
			using namespace cgp;
			vec3 a = {8.2f, 6.1f, -3.6};
			assert_cgp_no_msg(cgp::is_equal(min(a), -3.6f));
			assert_cgp_no_msg(cgp::is_equal(max(a),  8.2f));
			assert_cgp_no_msg(cgp::is_equal(sum(a),  8.2f+6.1f-3.6));
		}

		

	}
}