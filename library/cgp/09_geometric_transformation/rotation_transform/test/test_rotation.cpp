#include "test_rotation.hpp"

#include "cgp/01_base/base.hpp"
#include "../rotation_transform.hpp"

#include <iostream>
using namespace cgp;

#if defined(__linux__) || defined(__EMSCRIPTEN__)
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace cgp_test
{
	void test_rotation()
	{
		{
			rotation_transform R = rotation_transform::from_axis_angle(vec3(1,0,0),cgp::Pi/2.0f);
			assert_cgp_no_msg( type_str(R)=="rotation_transform" );
		}

		{
			assert_cgp_no_msg( is_equal(rotation_transform::from_axis_angle(vec3(1,0,0),cgp::Pi/2.0f)*vec3{1,0,0},vec3{1,0,0}) );
		}

		// Rotation between vectors
		{
			for (int ix = -1; ix <= 1; ++ix) {
				for (int iy = -1; iy <= 1; ++iy) {
					for (int iz = -1; iz <= 1; ++iz) {
						for (int ox = -1; ox <= 1; ++ox) {
							for (int oy = -1; oy <= 1; ++oy) {
								for (int oz = -1; oz <= 1; ++oz) {

									if( (ix!=0 || iy!=0 || iz!=0) && (ox!=0 || oy!=0 || oz!=0) ){
										vec3 const in  = normalize(vec3{float(ix), float(iy), float(iz)});
										vec3 const out = normalize(vec3{float(ox), float(oy), float(oz)});

										rotation_transform const R = rotation_transform::from_vector_transform(in, out);
										assert_cgp_no_msg( is_equal(R*in, out) );
									}
								}
							}
						}
					}
				}
			}




		}



	}
}