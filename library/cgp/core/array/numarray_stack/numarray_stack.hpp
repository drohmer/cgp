#pragma once


namespace cgp
{
    /** Base class for small fixed-size vectors (vec3, mat3, etc.).
     * buffer_stack structure is a generic fixed-size vector, essentially equivalent to a std::array.
     * In addition to std::array syntax, buffer_stack provides extra convenient functions (similar to buffer) for numerical vectors +, -, *, / as well as strict bounds checking.
     */
    // template <typename T, int N> struct buffer_stack;
}


#include "implementation/numarray_stack.hpp"

#include "implementation/numarray_stack2.hpp"
#include "implementation/numarray_stack3.hpp"
#include "implementation/numarray_stack4.hpp"

#include "special_types/special_types.hpp"