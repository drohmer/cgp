
// Automatically generated file using script update_test.py
// Last generation on: 2024-02-24

#include "cgp/cgp.hpp"
#include <iostream> 

#include "cgp/09_geometric_transformation/rotation_transform/test/test_rotation.hpp"
#include "cgp/04_grid_container/grid_stack/grid_stack_2D/test/test_grid_stack_2D.hpp"
#include "cgp/04_grid_container/grid/test/test_grid.hpp"
#include "cgp/02_numarray/numarray/test/test_numarray.hpp"
#include "cgp/02_numarray/numarray_stack/test/test_numarray_stack.hpp"
#include "cgp/19_camera_controller/test/test_camera_controller.hpp"
#include "cgp/06_mat/test/test_matrix_stack.hpp"
#include "cgp/06_mat/functions/test/test_vec_mat.hpp"


using namespace cgp;

int main(int, char* argv[])
{
	std::cout << "Run " << argv[0] << std::endl;

	cgp_test::test_rotation();
	cgp_test::test_grid_stack_2D();
	cgp_test::test_grid_2D();
	cgp_test::test_grid_3D();
	cgp_test::test_numarray();
	cgp_test::test_numarray_stack();
	cgp_test::test_camera_controller();
	cgp_test::test_matrix_stack();
	cgp_test::test_vec_mat();


	return 0;
}
