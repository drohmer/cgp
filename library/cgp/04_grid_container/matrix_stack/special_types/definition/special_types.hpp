#pragma once

#include "../../matrix_stack.hpp"



namespace cgp
{
	using mat2 = matrix_stack<float, 2, 2>;
	using mat3 = matrix_stack<float, 3, 3>;
	using mat4 = matrix_stack<float, 4, 4>;

	std::string type_str(mat2 const&);
	std::string type_str(mat3 const&);
	std::string type_str(mat4 const&);
}


