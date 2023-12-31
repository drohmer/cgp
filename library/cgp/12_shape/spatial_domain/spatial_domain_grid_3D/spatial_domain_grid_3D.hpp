#pragma once

#include "cgp/02_numarray/numarray.hpp"
#include "cgp/05_vec/vec.hpp"

namespace cgp {

	struct spatial_domain_grid_3D
	{
		vec3 center;
		vec3 length;
		int3 samples;

		spatial_domain_grid_3D();

		static spatial_domain_grid_3D from_center_length(vec3 const& center, vec3 const& length, int3 const& samples);
		static spatial_domain_grid_3D from_corners(vec3 const& corner_1, vec3 const& corner_2, int3 const& samples);

		/** Gives the spatial position corresponding to the index [kx,ky,kz] */
		vec3 position(int3 const& index) const;

		/** The index (kx,ky,kz) is converted in normalized position \in [0,1] */
		vec3 position_relative(int3 const& index) const;

		vec3 corner_min() const;
		vec3 corner_max() const;
		vec3 voxel_length() const;

		// Export segments for curve drawable
		numarray<vec3> export_segments_for_drawable_border() const;
		numarray<vec3> export_segments_for_drawable_voxel() const;
	};

}