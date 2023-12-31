
#include "cgp/01_base/base.hpp"
#include "special_types.hpp"




namespace cgp
{
    std::string type_str(int2 const&) { return "int2"; }
    std::string type_str(int3 const&) { return "int3"; }
    std::string type_str(int4 const&) { return "int4"; }
    std::string type_str(uint2 const&) { return "uint2"; }
    std::string type_str(uint3 const&) { return "uint3"; }
    std::string type_str(uint4 const&) { return "uint4"; }

    std::string type_str(vec2 const&) { return "vec2"; }
    std::string type_str(numarray_stack3<float> const&) { return "vec3"; }
    std::string type_str(vec4 const&) { return "vec4"; }
}