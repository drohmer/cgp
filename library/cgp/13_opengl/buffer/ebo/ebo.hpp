#pragma once

#include "../../buffer/buffer.hpp"
#include "cgp/02_numarray/numarray.hpp"


namespace cgp
{

	struct opengl_ebo_structure : opengl_gpu_buffer
	{
		void initialize_data_on_gpu(numarray<uint3> const& data);
	};




}