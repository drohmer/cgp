#include "field_function.hpp"

using namespace cgp;

// Parameterization Gaussian centered at point p0
static float gaussian(vec3 const& p, vec3 const& p0, float sigma)
{
	float const d = norm(p - p0);
	float const value = std::exp(-(d * d) / (sigma * sigma));
	return value;
}

float field_function_structure::operator()(cgp::vec3 const& p) const
{
	float value = 0.0f;
	value += sa * gaussian(p, pa, 1.0f);
	value += sb * gaussian(p, pb, 1.0f);
	value += sc * gaussian(p, pc, 1.0f);

	if (noise_magnitude > 0) {
		vec3 const offset = vec3{ noise_offset + 1000, 1000, 1000 };
		vec3 const p_noise = noise_scale * p + offset;
		value += noise_magnitude * noise_perlin(p_noise, noise_octave, noise_persistance);
	}

	return value;
}
