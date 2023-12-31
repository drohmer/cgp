#include "spatial_domain_grid_3D.hpp"

#include "cgp/01_base/base.hpp"

namespace cgp{

	vec3 center;
	vec3 length;
	int3 samples;

	spatial_domain_grid_3D::spatial_domain_grid_3D()
		:center(), length(), samples()
	{}

	spatial_domain_grid_3D spatial_domain_grid_3D::from_center_length(vec3 const& center, vec3 const& length, int3 const& samples)
	{
		spatial_domain_grid_3D domain;
		domain.center = center;
		domain.length = length;
		domain.samples = samples;
		return domain;
	}
	spatial_domain_grid_3D spatial_domain_grid_3D::from_corners(vec3 const& corner_1, vec3 const& corner_2, int3 const& samples)
	{
		spatial_domain_grid_3D domain;
		domain.center = (corner_1 + corner_2) / 2.0f;
		domain.length = abs(corner_1-corner_2);
		domain.samples = samples;
		return domain;
	}


	vec3 spatial_domain_grid_3D::position_relative(int3 const& index) const
	{
		return { index.x / (samples.x-1.0f), index.y/(samples.y-1.0f), index.z/(samples.z-1.0f) };
	}
	vec3 spatial_domain_grid_3D::position(int3 const& index) const
	{
		return corner_min() + position_relative(index) * length;
	}

	vec3 spatial_domain_grid_3D::corner_min() const
	{
		return center - length / 2.0f;
	}
	vec3 spatial_domain_grid_3D::corner_max() const
	{
		return center + length / 2.0f;
	}
	vec3 spatial_domain_grid_3D::voxel_length() const
	{
		return { length.x / (samples.x - 1), length.y / (samples.y - 1), length.z / (samples.z - 1) };
	}

	numarray<vec3> spatial_domain_grid_3D::export_segments_for_drawable_border() const
	{
		vec3 const& u = length;
		vec3 const p000 = corner_min();
		vec3 const p001 = p000 + vec3(0, 0, 1) * u;
		vec3 const p010 = p000 + vec3(0, 1, 0) * u;
		vec3 const p011 = p000 + vec3(0, 1, 1) * u;
		vec3 const p100 = p000 + vec3(1, 0, 0) * u;
		vec3 const p101 = p000 + vec3(1, 0, 1) * u;
		vec3 const p110 = p000 + vec3(1, 1, 0) * u;
		vec3 const p111 = p000 + vec3(1, 1, 1) * u;

		numarray<vec3> box;
		box.push_back(p000).push_back(p001);
		box.push_back(p001).push_back(p011);
		box.push_back(p011).push_back(p010);
		box.push_back(p010).push_back(p000);

		box.push_back(p100).push_back(p101);
		box.push_back(p101).push_back(p111);
		box.push_back(p111).push_back(p110);
		box.push_back(p110).push_back(p100);

		box.push_back(p000).push_back(p100);
		box.push_back(p001).push_back(p101);
		box.push_back(p011).push_back(p111);
		box.push_back(p010).push_back(p110);

		return box;
	}

	numarray<vec3> spatial_domain_grid_3D::export_segments_for_drawable_voxel() const
	{
		numarray<vec3> g;

		vec3 const p0 = corner_min();
		vec3 const& L = length;
		vec3 const dL = voxel_length();
		int3 const& s = samples;

		for (int kx = 0; kx < s.x; ++kx) {
			for (int ky = 0; ky < s.y; ++ky) {
				vec3 const p = p0 + dL * vec3(kx, ky, 0);
				g.push_back(p).push_back(p + L * vec3(0, 0, 1));
			}
		}

		for (int kx = 0; kx < s.x; ++kx) {
			for (int kz = 0; kz < s.z; ++kz) {
				vec3 const p = p0 + dL * vec3(kx, 0, kz);
				g.push_back(p).push_back(p + L * vec3(0, 1, 0));
			}
		}

		for (int ky = 0; ky < s.y; ++ky) {
			for (int kz = 0; kz < s.z; ++kz) {
				vec3 const p = p0 + dL * vec3(0, ky, kz);
				g.push_back(p).push_back(p + L * vec3(1, 0, 0));
			}
		}


		return g;
	}



}