#include "spatial_domain_grid_3D.hpp"

#include "cgp/core/base/base.hpp"

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


}