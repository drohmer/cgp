#include "cgp/01_base/base.hpp"
#include "../camera_controller.hpp"


#include <iostream>

#if defined(__linux__) || defined(__EMSCRIPTEN__)
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace cgp_test 
{

	void test_camera_controller()
	{
		{
			cgp::camera_controller_orbit controller;
			controller.look_at({ 1,0,0 }, { 0,0,0 }, { 0,0,1 });
			assert_cgp_no_msg( is_equal(controller.camera_model.front(), {-1, 0, 0}) );
			assert_cgp_no_msg(is_equal(controller.camera_model.position(), { 1, 0, 0 }));

			controller.look_at({ -1,0,0 }, { 0,0,0 }, { 0,0,1 });
			assert_cgp_no_msg(is_equal(controller.camera_model.front(), { 1, 0, 0 }));
			assert_cgp_no_msg(is_equal(controller.camera_model.position(), { -1, 0, 0 }));

			controller.look_at({ 0,1,0 }, { 0,0,0 }, { 0,0,1 });
			assert_cgp_no_msg(is_equal(controller.camera_model.front(), { 0, -1, 0 }));
			assert_cgp_no_msg(is_equal(controller.camera_model.position(), { 0, 1, 0 }));

			float const sqrt2 = std::sqrt(2.0f);
			controller.look_at({ 1,1,0 }, { 0,0,0 }, { 0,0,1 });
			assert_cgp_no_msg(is_equal(controller.camera_model.front(), { -1/sqrt2, -1/sqrt2, 0 }));
			assert_cgp_no_msg(is_equal(controller.camera_model.position(), { 1, 1, 0 }));
		}

		{
			cgp::camera_controller_orbit_euler controller;
			controller.look_at({ 1,0,0 }, { 0,0,0 }, { 0,0,1 });
			assert_cgp_no_msg(is_equal(controller.camera_model.front(), { -1, 0, 0 }));
			assert_cgp_no_msg(is_equal(controller.camera_model.position(), { 1, 0, 0 }));

			controller.look_at({ -1,0,0 }, { 0,0,0 }, { 0,0,1 });
			assert_cgp_no_msg(is_equal(controller.camera_model.front(), { 1, 0, 0 }));
			assert_cgp_no_msg(is_equal(controller.camera_model.position(), { -1, 0, 0 }));

			controller.look_at({ 0,1,0 }, { 0,0,0 }, { 0,0,1 });
			assert_cgp_no_msg(is_equal(controller.camera_model.front(), { 0, -1, 0 }));
			assert_cgp_no_msg(is_equal(controller.camera_model.position(), { 0, 1, 0 }));

			float const sqrt2 = std::sqrt(2.0f);
			controller.look_at({ 1,1,0 }, { 0,0,0 }, { 0,0,1 });
			assert_cgp_no_msg(is_equal(controller.camera_model.front(), { -1 / sqrt2, -1 / sqrt2, 0 }));
			assert_cgp_no_msg(is_equal(controller.camera_model.position(), { 1, 1, 0 }));
		}

		{
			cgp::camera_controller_first_person controller;
			controller.look_at({ 1, 0, 0 }, {0,0,0}, {0,0,1});
			assert_cgp_no_msg(is_equal(controller.camera_model.front(), { -1, 0, 0 }));
			assert_cgp_no_msg(is_equal(controller.camera_model.position(), { 1, 0, 0 }));

			controller.look_at({ -1,0,0 }, { 0,0,0 }, { 0,0,1 });
			assert_cgp_no_msg(is_equal(controller.camera_model.front(), { 1, 0, 0 }));
			assert_cgp_no_msg(is_equal(controller.camera_model.position(), { -1, 0, 0 }));

			controller.look_at({ 0,1,0 }, { 0,0,0 }, { 0,0,1 });
			assert_cgp_no_msg(is_equal(controller.camera_model.front(), { 0, -1, 0 }));
			assert_cgp_no_msg(is_equal(controller.camera_model.position(), { 0, 1, 0 }));

			float const sqrt2 = std::sqrt(2.0f);
			controller.look_at({ 1,1,0 }, { 0,0,0 }, { 0,0,1 });
			assert_cgp_no_msg(is_equal(controller.camera_model.front(), { -1 / sqrt2, -1 / sqrt2, 0 }));
			assert_cgp_no_msg(is_equal(controller.camera_model.position(), { 1, 1, 0 }));
		}

		{
			cgp::camera_controller_first_person_euler controller;
			controller.look_at({ 1, 0, 0 }, { 0,0,0 }, { 0,0,1 });
			assert_cgp_no_msg(is_equal(controller.camera_model.front(), { -1, 0, 0 }));
			assert_cgp_no_msg(is_equal(controller.camera_model.position(), { 1, 0, 0 }));

			controller.look_at({ -1,0,0 }, { 0,0,0 }, { 0,0,1 });
			assert_cgp_no_msg(is_equal(controller.camera_model.front(), { 1, 0, 0 }));
			assert_cgp_no_msg(is_equal(controller.camera_model.position(), { -1, 0, 0 }));

			controller.look_at({ 0,1,0 }, { 0,0,0 }, { 0,0,1 });
			assert_cgp_no_msg(is_equal(controller.camera_model.front(), { 0, -1, 0 }));
			assert_cgp_no_msg(is_equal(controller.camera_model.position(), { 0, 1, 0 }));

			float const sqrt2 = std::sqrt(2.0f);
			controller.look_at({ 1,1,0 }, { 0,0,0 }, { 0,0,1 });
			assert_cgp_no_msg(is_equal(controller.camera_model.front(), { -1.0f / sqrt2, -1.0f / sqrt2, 0 }));
			assert_cgp_no_msg(is_equal(controller.camera_model.position(), { 1, 1, 0 }));
		}


	}
}