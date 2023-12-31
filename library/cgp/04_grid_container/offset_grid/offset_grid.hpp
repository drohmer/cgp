#pragma once

#include <tuple>
#include <cstddef>

#include "cgp/02_numarray/numarray_stack/numarray_stack.hpp"

namespace cgp
{

	// 1D offset corresponding to the index (k1,k2) in a 2D grid with static size
	template <int N1> size_t offset_grid_stack(int k1, int k2);

	// Index (k1,k2) corresponding to a given offset in a 2D grid with static size
	template <int N1> std::pair<int, int> index_grid_from_offset_stack(int offset);


	// 1D offset corresponding to the index (k1,k2) in a 2D grid
	inline int offset_grid(int k1, int k2, int N1);
	// 1D offset corresponding to the index (k1,k2) in a 2D grid
	int offset_grid(int2 const& k, int N1);

	// 1D offset corresponding to the index (k1,k2,k3) in a 2D grid
	inline int offset_grid(int k1, int k2, int k3, int N1, int N2);
	// 1D offset corresponding to the index (k1,k2,k3) in a 2D grid
	int offset_grid(int3 const& k, int N1, int N2);
	
	// Index (k1,k2) corresponding to a given offset in a 2D grid with static size
	int2 index_grid_from_offset(int offset, int N1);
	// Index (k1,k2,k3) corresponding to a given offset in a 3D grid with static size
	int3 index_grid_from_offset(int offset, int N1, int N2);

}

namespace cgp
{

	template <int N1> size_t offset_grid_stack(int k1, int k2)
	{
		return k1 + N1 * k2;
	}

	template <int N1> std::pair<int, int> index_grid_from_offset_stack(int offset)
	{
		int const k1 = offset / N1;
		int const k2 = offset - k1 * N1;
		return { k1,k2 };
	}

	inline int offset_grid(int k1, int k2, int N1)
	{
		return k1 + N1 * k2;
	}
	inline int offset_grid(int k1, int k2, int k3, int N1, int N2)
	{
		return k1 + N1 * (k2 + N2 * k3);
	}
}