#include "field_function.hpp"


using namespace cgp;

float field_evaluate(vec3 const& p);

grid_3D<float> compute_scalar_field(spatial_domain_grid_3D const& domain)
{
	grid_3D<float> field;
	field.resize(domain.samples);

	// Fill the discrete field values
	for (int kz = 0; kz < domain.samples.z; kz++) {
		for (int ky = 0; ky < domain.samples.y; ky++) {
			for (int kx = 0; kx < domain.samples.x; kx++) {

				vec3 const p = domain.position({ kx, ky, kz });
				field(kx, ky, kz) = field_evaluate(p);

			}
		}
	}

	return field;
}


// Parameterization of a basic "blob"-function - Gaussian centered at point p0
float blob(vec3 const& p, vec3 const& p0)
{
	float const d = norm(p - p0);
	float const value = std::exp(-d * d);
	return value;
}

// Evaluate the field at an arbitrary position in space
float field_evaluate(vec3 const& p)
{
	float value = 0.0f;
	value += blob(p, { -1,0,0 });
	value += blob(p, { +1,0,0 });

	return value;
}