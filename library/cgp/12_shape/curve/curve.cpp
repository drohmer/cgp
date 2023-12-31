#include "cgp/01_base/base.hpp"
#include "cgp/09_geometric_transformation/rotation_transform/rotation_transform.hpp"
#include "curve.hpp"


namespace cgp
{
	numarray<vec3> curve_primitive_circle(float radius, vec3 const& center, vec3 const& normal, int N_sample)
	{
		assert_cgp(N_sample>2, "Circle curve should have N_sample>2");

		numarray<vec3> curve;
		rotation_transform const R = rotation_transform::from_vector_transform({0,0,1}, normal);
		for (int k = 0; k < N_sample; ++k)
		{
			float const u = k/(N_sample-1.0f);
			vec3 const p = {std::cos(2*Pi*u), std::sin(2*Pi*u), 0.0f};
			curve.push_back(R*(radius*p)+center);
		}

		return curve;
	}

	numarray<vec3> curve_to_segments(numarray<vec3> const& curve_in)
	{
		assert_cgp(curve_in.size() >= 2, "Curve should have N_sample>=2");

		numarray<vec3> segments;
		for (int k = 1; k < curve_in.size(); ++k) {
			segments.push_back(curve_in[k - 1]);
			segments.push_back(curve_in[k]);
		}

		return segments;
	}
}