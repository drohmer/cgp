#pragma once

#include "cgp/02_numarray/numarray_stack/numarray_stack.hpp"

namespace cgp
{
	struct quaternion : vec4
	{
        using vec4::vec4;
	};

	std::string type_str(quaternion const&);

    quaternion conjugate(quaternion const& q);
    quaternion inverse(quaternion const& q);

    quaternion& operator*=(quaternion& a, quaternion const& b);
    quaternion  operator*(quaternion const& a, quaternion const& b);
    quaternion  operator*(quaternion const& a, float b);
    quaternion  operator*(float a, quaternion const& b);

    quaternion& operator/=(quaternion& a, quaternion const& b);
    quaternion  operator/(quaternion const& a, quaternion const& b);
    quaternion  operator/(quaternion const& a, float b);
    quaternion  operator/(float a, quaternion const& b);

    quaternion& operator+=(quaternion& a, quaternion const& b);
    quaternion operator+(quaternion const& a, quaternion const& b);
    quaternion& operator-=(quaternion& a, quaternion const& b);
    quaternion operator-(quaternion const& a, quaternion const& b);

    quaternion normalize(quaternion const& q);

	std::istream& operator>>(std::istream& stream, quaternion& data);
}