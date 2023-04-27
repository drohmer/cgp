
// Automatically generated file using script update_test.py
// Last generation on: 2023-04-26

#include "cgp/cgp.hpp"
#include <iostream> 

#include "cgp/core/array/numarray/test/test_numarray.hpp"
#include "cgp/core/array/numarray_stack/test/test_numarray_stack.hpp"
#include "cgp/core/containers/grid/test/test_grid.hpp"
#include "cgp/core/containers/grid_stack/grid_stack_2D/test/test_grid_stack_2D.hpp"
#include "cgp/geometry/mat/test/test_matrix_stack.hpp"
#include "cgp/geometry/mat/functions/test/test_vec_mat.hpp"
#include "cgp/geometry/transform/rotation_transform/test/test_rotation.hpp"
#include "cgp/graphics/camera/camera_controller/test/test_camera_controller.hpp"


using namespace cgp;

int main(int, char* argv[])
{
	std::cout << "Run " << argv[0] << std::endl;

	cgp_test::test_numarray();
	cgp_test::test_numarray_stack();
	cgp_test::test_grid_2D();
	cgp_test::test_grid_3D();
	cgp_test::test_grid_stack_2D();
	cgp_test::test_matrix_stack();
	cgp_test::test_vec_mat();
	cgp_test::test_rotation();
	cgp_test::test_camera_controller();


	return 0;
}
