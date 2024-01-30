#pragma once

#include "cgp/04_grid_container/matrix_stack/matrix_stack.hpp"
#include "cgp/05_vec/vec.hpp"

// mat4 is an alias on matrix_stack<float, 4, 4>
// Its generic implementation can be found in file cgp/math/matrix/matrix_stack/matrix_stack.hpp

// A mat4 struct can be seen as
//   struct mat4 { buffer_stack< vec4, 4> }
//   (with additional functions)


namespace cgp
{
    struct quaternion;
    using mat4 = matrix_stack<float, 4, 4>;
    using mat3 = matrix_stack<float, 3, 3>;

    /** Container for 4x4 matrix of floats
     *
     * Elements of mat4 are stored contiguously in stack memory as array of array and remain fully compatible with std::array and pointers.
     * Elements are stored along rows
     * matrix[0]     ( xx:(0,0) xy:(0,1) xz:(0,2) xw:(0,3) )
     * matrix[1]     ( yx:(1,0) yy:(1,1) yz:(1,2) yw:(1,3) )
     * matrix[2]     ( zx:(2,0) zy:(2,1) zz:(2,2) zw:(2,3) )
     * matrix[3]     ( wx:(3,0) wy:(3,1) wz:(3,2) ww:(3,3) )
     **/
    template <>
    struct matrix_stack<float, 4, 4>
    {
        /** Internal storage as a 1D buffer */
        numarray_stack< numarray_stack<float, 4>, 4> data;


        // ******************************************************* //
        //  Constructors
        // ******************************************************* //
        matrix_stack();
        matrix_stack(numarray_stack< numarray_stack<float, 4>, 4> const& elements);
        matrix_stack(vec4 const& row_1, vec4 const& row_2, vec4 const& row_3, vec4 const& row_4);
        matrix_stack(numarray_stack<float, 16> const& elements);
        matrix_stack(
            float xx, float xy, float xz, float xw,
            float yx, float yy, float yz, float yw,
            float zx, float zy, float zz, float zw,
            float wx, float wy, float wz, float ww);

        // Special construction from a 3x3 matrix
        //  Build the block matrix
        //  |M 0|
        //  |0 1|                                    
        explicit matrix_stack(mat3 const& M);

        // Construct from a matrix with different size.
        //  Consider the min between (N1,N1_arg) and (N2,N2_arg)
        template <int N1_arg, int N2_arg>
        explicit matrix_stack(matrix_stack<float, N1_arg, N2_arg> const& M);

        // Build as a diagonal matrix (glm compatibility)
        explicit matrix_stack(float value);
        explicit matrix_stack(float xx, float yy, float zz, float ww=1.0f);

        matrix_stack(std::initializer_list<float> const& arg);
        matrix_stack(std::initializer_list<numarray_stack<float, 4> > const& arg);



        // ******************************************************* //
        //  Special matrix build
        // ******************************************************* //

        // Build the identity matrix
        static mat4 build_identity();
        // Build a zero matrix (similar to default constructor)
        static mat4 build_zero();

        // Build a matrix filled with a single value
        static mat4 build_constant(float value);

        // Build a diagonal matrix from a constant value or 4 floats
        static mat4 build_diagonal(float value);
        static mat4 build_diagonal(vec4 const& arg);
        static mat4 build_diagonal(float x, float y, float z, float w);

        // Build a scaling matrix from a constant value
        static mat4 build_scaling(float value);
        // Build a diagonal matrix with 3 floats
        static mat4 build_scaling(vec3 const& arg);
        // Build a diagonal matrix with 3 floats
        static mat4 build_scaling(float x, float y, float z);


        // Build an affine matrix from mat3 and translation parts
        //  | linear tr|
        //  |   0    1 |  
        static mat4 build_affine(mat3 const& linear, vec3 const& tr);

        // Build a matrix representing a translation
        // | 1 tr |
        // | 0  1 |
        static mat4 build_translation(vec3 const& tr);
        // Build a matrix representing a translation. Similar to build_translation(vec3(x,y,z))
        static mat4 build_translation(float x, float y, float z);

        // Build a matrix representing a linear transform
        // | linear 0 |
        // |   0    1 |
        static mat4 build_linear(mat3 const& M);

        // Build a matrix representing a rotation from an axis and angle parameter
        static mat4 build_rotation_from_axis_angle(vec3 const& axis, float angle);
        // Build a matrix representing a rotation from a unit quaternion
        static mat4 build_rotation_from_quaternion(quaternion const& q);

        // ******************************************************* //
        //  Fill and set with special matrix form
        // ******************************************************* //

        mat4& set_identity();
        mat4& set_zeros();
        mat4& set_diagonal(float value);
        mat4& set_diagonal(float xx, float yy, float zz, float ww);

        // ******************************************************* //
        //  Special manipulation as affine transform
        // ******************************************************* //

        // Set the linear part of the matrix (3x3 top-left corner)
        mat4& set_block_linear(mat3 const& L);
        // Set the linear part of the matrix to be a scaling
        mat4& set_block_linear_as_scaling(vec3 const& s);
        mat4& set_block_linear_as_scaling(float sx, float sy, float sz);
        mat4& set_block_linear_as_scaling(float s);
        // Set the linear part of the matrix (3x3 top-left corner) to be a rotation parameterized by an axis and angle
        mat4& set_block_linear_as_rotation(vec3 const& axis, float angle);

        // Set the translation part of the matrix (1x3 top-right corner)
        mat4& set_block_translation(vec3 const& tr);
        mat4& set_block_translation(float x, float y, float z);

        // Return the translation part of a 4x4 matrix vec3(m(2,0),m(2,1),m(2,2))
        vec3 get_block_translation() const;
        // Return the 3x3 matrix component out of the 4x4 matrix. Similar to the call mat3(M4x4).
        mat3 get_block_linear() const;

        // ******************************************************* //
        //  Apply internal transformation
        // ******************************************************* //

        mat4& apply_scaling_to_block_linear(float s);
        mat4& apply_scaling_to_block_translation(float s);
        mat4& apply_scaling(float s);

        mat4& apply_translation(vec3 const& xyz);
        // linear_part = T * linear_part
        mat4& apply_transform_to_block_linear(mat3 const& T);

        // ******************************************************* //
        //  Size and fill
        // ******************************************************* //

        /** Return 16 */
        int size() const;
        /** Return {4,4} */
        int2 dimension() const;
        /** Fill all elements of the grid_2D with the same element*/
        mat4& fill(float value);


        // ******************************************************* //
        //  Get/Set
        // ******************************************************* //

        vec4 col_x() const;
        vec4 col_y() const;
        vec4 col_z() const;
        vec4 col_w() const;
        vec4 const& row_x() const;
        vec4 const& row_y() const;
        vec4 const& row_z() const;
        vec4 const& row_w() const;
        vec4& row_x();
        vec4& row_y();
        vec4& row_z();
        vec4& row_w();

        vec3 row_x_vec3() const;
        vec3 row_y_vec3() const;
        vec3 row_z_vec3() const;
        vec3 row_w_vec3() const;
        vec3 col_x_vec3() const;
        vec3 col_y_vec3() const;
        vec3 col_z_vec3() const;
        vec3 col_w_vec3() const;


        /** Element access
         * Bound checking is performed unless cgp_NO_DEBUG is defined. */
        vec4 const& operator[](int k2) const;
        vec4& operator[](int k2);

        vec4 const& operator()(int k2) const;
        vec4& operator()(int k2);

        float const& operator()(int k1, int k2) const;
        float& operator()(int k1, int k2);

        float const& at_offset(int offset) const;
        float& at_offset(int offset);


        // Apply mat4 to a vec3 representing a 3D position:
        //  Similar to q = M*vec4(p,1.0); return q.xyz()/q.w()
        vec3 transform_position(vec3 const& pos_xyz1) const;

        // Apply mat4 to a vec3 representing a spatial vector:
        //  Similar to q = (M*vec4(p,0.0)).xyz();
        vec3 transform_vector(vec3 const& vec_xyz0) const;


        matrix_stack<float, 3, 3> remove_row_column(int k1, int k2) const;

        /** Set a block within the matrix from a specific offset
        *    @block: the matrix to be copied in the current one
        *    @offset: the offsets where the block has to be writter. Default value are (0,0). The block is written on the top-left corner.
        * Conditions:
        *     offset_1 + N1_arg < N1
        *     offset_2 + N2_arg < N2
        */
        template <int N1_arg, int N2_arg>
        matrix_stack<float, 4, 4>& set_block(matrix_stack<float, N1_arg, N2_arg> const& block, int offset_1 = 0, int offset_2 = 0);



        /** Iterators
        *  Iterators compatible with STL syntax and std::array */
        float* begin();
        float* end();
        float const* begin() const;
        float const* end() const;
        float const* cbegin() const;
        float const* cend() const;


        inline float const& at(int k1, int k2) const;
        inline float& at(int k1, int k2);

        float const& at_unsafe(int k1, int k2) const;
        float& at_unsafe(int k1, int k2);

        vec4 const& at_unsafe(int k1) const;
        vec4& at_unsafe(int k1);

        float const& at_offset_unsafe(int offset) const;
        float& at_offset_unsafe(int offset);



        // Compute the inverse of the mat4 structure assuming it is a rigid transform made of a rotation and translation part.
        mat4 inverse_assuming_rigid_transform() const;
    };

    mat4 operator*(mat4 const& a, mat4 const& b);
    mat4 operator*(float s, mat4 const& M);
    mat4& operator*=(mat4& a, mat4 const& b); // a = a*b
    mat4& operator*=(mat4& M, float s);
    mat4& operator+=(mat4& a, mat4 const& b);

}


namespace cgp
{
    template <int N1_arg, int N2_arg>
    matrix_stack<float, 4, 4>::matrix_stack(matrix_stack<float, N1_arg, N2_arg> const& M)
        :data()
    {
        int const N1m = std::min(int(4), N1_arg);
        int const N2m = std::min(int(4), N2_arg);
        for (int k1 = 0; k1 < N1m; ++k1)
            for (int k2 = 0; k2 < N2m; ++k2)
                at(k1, k2) = M.at(k1, k2);
    }

    template <int N1_arg, int N2_arg>
    matrix_stack<float, 4, 4>& matrix_stack<float, 4, 4>::set_block(matrix_stack<float, N1_arg, N2_arg> const& block, int offset_1, int offset_2)
    {
        static_assert(N1_arg < 4, "Block size is too large for the current matrix");
        static_assert(N2_arg < 4, "Block size is too large for the current matrix");
        assert_cgp(N1_arg + offset_1 < 4, "Block size exceed current matrix size");
        assert_cgp(N2_arg + offset_2 < 4, "Block size exceed current matrix size");

        for (int k1 = 0; k1 < N1_arg; ++k1) {
            int const idx_1 = k1 + offset_1;
            for (int k2 = 0; k2 < N2_arg; ++k2) {
                int const idx_2 = k2 + offset_2;
                at(idx_1, idx_2) = block.at(k1, k2);
            }
        }
        return *this;
    }


    float const& matrix_stack<float, 4, 4>::at(int k1, int k2) const
    {
        return *(begin() + k2 + 4 * k1);
    }
    float& matrix_stack<float, 4, 4>::at(int k1, int k2)
    {
        return *(begin() + k2 + 4 * k1);
    }



    

}