#pragma once

#include "cgp/02_numarray/numarray_stack/numarray_stack.hpp"

// vec4 is an alias on buffer_stack<float, 4>
// Its implementation can be found in file cgp/containers/buffer_stack/buffer_stack4.hpp

// A vec4 struct can be used as
//   struct vec4 { float x, y, z, w; }
//   (with additional functions handled as a buffer_stack)

