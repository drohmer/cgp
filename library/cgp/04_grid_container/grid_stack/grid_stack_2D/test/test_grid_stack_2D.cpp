#include "cgp/01_base/base.hpp"
#include "../grid_stack_2D.hpp"


#include <iostream>

namespace cgp_test 
{

	void test_grid_stack_2D()
	{
		{
			cgp::grid_stack_2D<int, 2, 3> a = { 1,2,3, 4,5,6 };
			
			assert_cgp_no_msg(a(0, 0) == 1); assert_cgp_no_msg(a(0, 1) == 2); assert_cgp_no_msg(a(0, 2) == 3);
			assert_cgp_no_msg(a(1, 0) == 4); assert_cgp_no_msg(a(1, 1) == 5); assert_cgp_no_msg(a(1, 2) == 6);

			assert_cgp_no_msg( cgp::is_equal(cgp::element<0, 0>(a), 1) ); assert_cgp_no_msg(cgp::is_equal(cgp::element<0, 1>(a), 2)); assert_cgp_no_msg(cgp::is_equal(cgp::element<0, 2>(a), 3));
			assert_cgp_no_msg( cgp::is_equal(cgp::element<1, 0>(a), 4)); assert_cgp_no_msg(cgp::is_equal(cgp::element<1, 1>(a), 5)); assert_cgp_no_msg(cgp::is_equal(cgp::element<1, 2>(a), 6));

			assert_cgp_no_msg(type_str(a) == "grid_stack_2D<int,2,3>");
		}

		{
			using namespace cgp;
			grid_stack_2D<int,2,3> const a = {1,2,3, 4,5,6};
			cgp::is_equal(a(1,2), 6);
			cgp::is_equal(a({1,2}), 6);
			cgp::is_equal(a[{1,2}], 6);

		}

	}
}