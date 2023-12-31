#pragma once

#include "../implementation/numarray_stack.hpp"
#include "../implementation/numarray_stack2.hpp"
#include "../implementation/numarray_stack3.hpp"
#include "../implementation/numarray_stack4.hpp"



namespace cgp
{
    /* ************************************************** */
    /*        Helper type declaraction                    */
    /* ************************************************** */
    template <typename T> using  numarray_stack2 = numarray_stack<T, 2>;
    template <typename T> using  numarray_stack3 = numarray_stack<T, 3>;
    template <typename T> using  numarray_stack4 = numarray_stack<T, 4>;

    using int2 = numarray_stack2<int>;
    using int3 = numarray_stack3<int>;
    using int4 = numarray_stack4<int>;

    using uint2 = numarray_stack2<unsigned int>;
    using uint3 = numarray_stack3<unsigned int>;
    using uint4 = numarray_stack4<unsigned int>;

    using vec2 = numarray_stack2<float>;
    using vec3 = numarray_stack3<float>;
    using vec4 = numarray_stack4<float>;


    std::string type_str(int2 const&);
    std::string type_str(int3 const&);
    std::string type_str(int4 const&);
    std::string type_str(uint2 const&);
    std::string type_str(uint3 const&);
    std::string type_str(uint4 const&);

    std::string type_str(vec2 const&);
    std::string type_str(numarray_stack3<float> const&);
    std::string type_str(vec4 const&);

}