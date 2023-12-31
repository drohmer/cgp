#include "cgp/01_base/base.hpp"
#include "projection.hpp"


namespace cgp
{
	mat4 projection_perspective(float fov, float aspect, float z_near, float z_far)
    {
	    const float fy = 1/std::tan(fov/2);
        const float fx = fy / aspect;
        const float L = z_near-z_far;

        const float C = (z_far+z_near)/L;
        const float D = (2*z_far*z_near)/L;

	    return mat4{
            fx,0,0,0,
            0,fy,0,0,
            0,0,C,D,
            0,0,-1,0
        };
    }

    mat4 projection_perspective_inverse(float fov, float aspect, float z_near, float z_far)
    {
        const float fy = 1/std::tan(fov/2);
        const float fx = fy/aspect;
        const float L = z_near-z_far;

        const float C = (z_far+z_near)/L;
        const float D = (2*z_far*z_near)/L;

	    return mat4{
            1/fx,0,0,0,
            0,1/fy,0,0,
            0,0,0,-1,
            0,0,1/D,C/D
        };
    }


    mat4 projection_orthographic(float left, float right, float bottom, float top, float z_near, float z_far)
    {
        return mat4{
            2/(right-left), 0, 0, -(right+left)/(right-left), 
            0, 2/(top-bottom), 0, -(top+bottom)/(top-bottom), 
            0, 0, -2/(z_far-z_near), -(z_far+z_near)/(z_far-z_near),
            0,0,0, 1
        };
    }
    mat4 projection_orthographic_inverse(float left, float right, float bottom, float top, float z_near, float z_far)
    {
        return mat4{
            (right-left)/2, 0, 0, (right+left)/2, 
            0, (top-bottom)/2, 0, (top+bottom)/2, 
            0, 0, -(z_far-z_near)/2, (z_far+z_near)/2,
            0,0,0, 1
        };
    }
}