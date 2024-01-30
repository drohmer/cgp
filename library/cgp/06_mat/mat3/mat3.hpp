#pragma once

#include "cgp/04_grid_container/matrix_stack/matrix_stack.hpp"
#include "cgp/05_vec/vec.hpp"


// mat3 is an alias on matrix_stack<float, 3, 3>
// Its generic implementation can be found in file cgp/math/matrix/matrix_stack/matrix_stack.hpp

// A mat3 struct can be seen as
//   struct mat3 { buffer_stack< vec3, 3> }
//   (with additional functions)

namespace cgp
{
    using mat3 = matrix_stack<float, 3, 3>;
    struct quaternion;

    /** Container for 3x3 matrix of floats
    *
    * Elements of mat2 are stored contiguously in stack memory as array of array
    * Elements are stored along rows
    * matrix[0]     ( xx:(0,0) xy:(0,1) xz:(0,2) )
    * matrix[1]     ( yx:(1,0) yy:(1,1) yz:(1,2) )
    * matrix[2]     ( zx:(2,0) zy:(2,1) zz:(2,2) )
    **/
    template <>
    struct matrix_stack<float, 3, 3>
    {
        /** Internal storage as a 1D buffer */
        numarray_stack< numarray_stack<float, 3>, 3> data;

        // ******************************************************* //
        //  Constructors
        // ******************************************************* //
        matrix_stack();
        matrix_stack(numarray_stack< vec3, 3> const& elements);
        matrix_stack(vec3 const& row_1, vec3 const& row_2, vec3 const& row_3);
        matrix_stack(numarray_stack<float, 9> const& elements);
        matrix_stack(
            float xx, float xy, float xz,
            float yx, float yy, float yz,
            float zx, float zy, float zz);

        // Construct from a matrix with different size.
        //  Consider the min between (N1,N1_arg) and (N2,N2_arg)
        template <int N1_arg, int N2_arg>
        explicit matrix_stack(matrix_stack<float, N1_arg, N2_arg> const& M);

        // Build as a diagonal matrix (glm compatibility)
        explicit matrix_stack(float value);
        explicit matrix_stack(float xx, float yy, float zz);

        matrix_stack(std::initializer_list<float> const& arg);
        matrix_stack(std::initializer_list<vec3> const& arg);


        // ******************************************************* //
        //  Size and fill
        // ******************************************************* //

        // Return 9
        int size() const;
        // Return {3,3}
        int2 dimension() const;
        // Fill all elements with a constant value
        mat3& fill(float value);


        // ******************************************************* //
        //  Special matrix build
        // ******************************************************* //

        // Build the identity matrix
        static mat3 build_identity();
        // Build a zero matrix (similar to default constructor)
        static mat3 build_zero();

        // Build a matrix filled with a single value
        static mat3 build_constant(float value);

        // Build a diagonal matrix from a constant value or 2 floats
        static mat3 build_diagonal(float value);
        static mat3 build_diagonal(vec3 const& arg);
        static mat3 build_diagonal(float x, float y, float z);

        // Build a scaling matrix from a constant value or 2 floats 
        //  (similar to build_diagonal)
        static mat3 build_scaling(float value);
        static mat3 build_scaling(vec3 const& arg);
        static mat3 build_scaling(float x, float y, float z);

        // Build a matrix representing a rotation from an axis and angle parameter
        static mat3 build_rotation_from_axis_angle(vec3 const& axis, float angle);
        // Build a matrix representing a rotation from a unit quaternion
        static mat3 build_rotation_from_quaternion(quaternion const& q);



        // ******************************************************* //
        //  Get/Set
        // ******************************************************* //

        vec3 const& operator[](int k2) const;
        vec3& operator[](int k2);

        vec3 const& operator()(int k2) const;
        vec3& operator()(int k2);

        float const& operator()(int k1, int k2) const;
        float& operator()(int k1, int k2);

        inline vec3 const& at(int k2) const;
        inline vec3& at(int k2);

        inline float const& at(int k1, int k2) const;
        inline float& at(int k1, int k2);

        inline float const& at_offset(int offset) const;
        inline float& at_offset(int offset);


        /** Iterators
        *  Iterators compatible with STL syntax and std::array */
        inline float* begin();
        inline float* end();
        inline float const* begin() const;
        inline float const* end() const;
        inline float const* cbegin() const;
        inline float const* cend() const;




    };
}

namespace cgp
{
    // Construct from a matrix with different size.
    //  Consider the min between (N1,N1_arg) and (N2,N2_arg)
    template <int N1_arg, int N2_arg>
    mat3::matrix_stack(matrix_stack<float, N1_arg, N2_arg> const& M)
        :data()
    {
        int const N1m = std::min(3, N1_arg);
        int const N2m = std::min(3, N2_arg);
        for (int k1 = 0; k1 < N1m; ++k1)
            for (int k2 = 0; k2 < N2m; ++k2)
                at(k1, k2) = M.at(k1, k2);
    }

    float const& mat3::at(int k1, int k2) const
    {
        return *(begin() + k2 + 3 * k1);
    }
    float& mat3::at(int k1, int k2)
    {
        return *(begin() + k2 + 3 * k1);
    }

    vec3 const& mat3::at(int k2) const
    {
        return data.at(k2);
    }
    vec3& mat3::at(int k2)
    {
        return data.at(k2);
    }

    float const& mat3::at_offset(int offset) const
    {
        return *(begin() + offset);
    }
    float& mat3::at_offset(int offset) 
    {
        return *(begin() + offset);
    }

    float* mat3::begin()
    {
        return &data.x.x;
    }
    float* mat3::end()
    {
        return &data.z.z+1;
    }
    float const* mat3::begin() const
    {
        return &data.x.x;
    }
    float const* mat3::end() const
    {
        return &data.z.z + 1;
    }
    float const* mat3::cbegin() const
    {
        return &data.x.x;
    }
    float const* mat3::cend() const
    {
        return &data.z.z + 1;
    }

}