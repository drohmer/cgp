#include "cgp/01_base/base.hpp"
#include "quaternion.hpp"
#include <iostream>

namespace cgp
{
	std::string type_str(quaternion const&)
	{
		return "quaternion";
	}

    quaternion conjugate(quaternion const& q)
    {
        return quaternion{ -q.xyz(), q.w };
    }
    quaternion inverse(quaternion const& q)
    {
        return conjugate(q) / dot(q, q);
    }


    quaternion& operator*=(quaternion& a, quaternion const& b)
    {
        a = a * b;
        return a;
    }

    quaternion  operator*(quaternion const& a, quaternion const& b)
    {
        return quaternion{
            a.x * b.w + a.w * b.x + a.y * b.z - a.z * b.y,
            a.y * b.w + a.w * b.y + a.z * b.x - a.x * b.z,
            a.z * b.w + a.w * b.z + a.x * b.y - a.y * b.x,
            a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z
        };
    }

    quaternion operator*(quaternion const& a, float b)
    {
        quaternion res = a;
        res *= b;
        return res;
    }
    quaternion operator*(float a, quaternion const& b)
    {
        quaternion res = b;
        res *= a;
        return res;
    }


    quaternion& operator/=(quaternion& a, quaternion const& b)
    {
        a = a / b;
        return a;
    }
    quaternion  operator/(quaternion const& a, quaternion const& b)
    {
        return a * inverse(b);
    }
    quaternion operator/(quaternion const& a, float b)
    {
        quaternion res = a;
        res /= b;
        return res;
    }
    quaternion operator/(float a, quaternion const& b)
    {
        quaternion res = inverse(b);
        return a * res;
    }

    quaternion& operator+=(quaternion& a, quaternion const& b)
    {
        static_cast<vec4&>(a) += b;
        return a;
    }
    quaternion operator+(quaternion const& a, quaternion const& b)
    {
        quaternion res = a;
        res += b;
        return res;
    }
    quaternion& operator-=(quaternion& a, quaternion const& b)
    {
        static_cast<vec4&>(a) -= b;
        return a;
    }
    quaternion operator-(quaternion const& a, quaternion const& b)
    {
        quaternion res = a;
        res -= b;
        return res;
    }
    quaternion normalize(quaternion const& q)
    {
        return q/norm(q);
    }

    std::istream& operator>>(std::istream& stream, quaternion& data)
    {
        stream >> data.x;
        stream >> data.y;
        stream >> data.z;
        stream >> data.w;

        return stream;
    }
}