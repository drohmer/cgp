#include "cgp/01_base/base.hpp"
#include "offset_grid.hpp"



namespace cgp
{

	int2 index_grid_from_offset(int offset, int N1)
	{
		int const k2 = offset / N1;
		int const k1 = offset - k2 * N1;
		return { k1,k2 };
	}
	int3 index_grid_from_offset(int offset, int N1, int N2)
	{
		int const k3 = offset / (N1*N2);
		int const k2 = (offset - N1 * N2 * k3) / N1;
		int const k1 = offset - N1 * (N2 * k3 - k2);

		return { k1,k2,k3 };
	}

	int offset_grid(int3 const& k, int N1, int N2)
	{
		return offset_grid(k.x, k.y, k.z, N1, N2);
	}
	int offset_grid(int2 const& k, int N1)
	{
		return offset_grid(k.x, k.y, N1);
	}
}