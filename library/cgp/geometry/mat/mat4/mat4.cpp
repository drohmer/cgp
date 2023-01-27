#include "cgp/core/base/base.hpp"

#include "mat4.hpp"
#include "cgp/geometry/transform/rotation_transform/rotation_transform.hpp"

namespace cgp
{
    mat4::matrix_stack()
        :data()
    {}

    mat4::matrix_stack(numarray_stack< vec4, 4> const& elements)
        : data(elements)
    {}

    mat4::matrix_stack(vec4 const& row_1, vec4 const& row_2, vec4 const& row_3, vec4 const& row_4)
        :data({ row_1,row_2,row_3,row_4 })
    {}

    mat4::matrix_stack(numarray_stack<float, 16> const& elements)
        : data({
            vec4{ get<0>(elements),get<1>(elements),get<2>(elements),get<3>(elements)},
            vec4{ get<4>(elements),get<5>(elements),get<6>(elements),get<7>(elements)},
            vec4{ get<8>(elements),get<9>(elements),get<10>(elements),get<11>(elements)},
            vec4{ get<12>(elements),get<13>(elements),get<14>(elements),get<15>(elements)} })
    {}


    mat4::matrix_stack(
        float xx, float xy, float xz, float xw,
        float yx, float yy, float yz, float yw,
        float zx, float zy, float zz, float zw,
        float wx, float wy, float wz, float ww)
        : data(
            vec4(xx, xy, xz, xw),
            vec4(yx, yy, yz, yw),
            vec4(zx, zy, zz, zw),
            vec4(wx, wy, wz, ww)
        )
    {}

    mat4::matrix_stack(mat3 const& M)
        :data({
        vec4(get<0,0>(M), get<0,1>(M), get<0,2>(M), 0),
        vec4(get<1,0>(M), get<1,1>(M), get<1,2>(M), 0),
        vec4(get<2,0>(M), get<2,1>(M), get<2,2>(M), 0),
        vec4(0, 0, 0, 1) })
    {}


    mat4::matrix_stack(std::initializer_list<float> const& arg)
        :data()
    {
        assert_cgp(arg.size() >= 16, "Insufficient size to initialize mat4");
        auto it_arg = arg.begin();

        for (int k1 = 0; k1 < 4; ++k1) {
            for (int k2 = 0; k2 < 4; ++k2) {
                at(k1, k2) = *it_arg;
                ++it_arg;
            }
        }
    }
    mat4::matrix_stack(std::initializer_list<vec4> const& arg)
    {
        assert_cgp(arg.size() >= 4, "Insufficient size to initialize mat4");
        auto it_arg = arg.begin();
        
        data.x = *it_arg; ++it_arg;
        data.y = *it_arg; ++it_arg;
        data.z = *it_arg; ++it_arg;
        data.w = *it_arg;
    }





    mat4 matrix_stack<float, 4, 4>::build_identity()
    {
        return matrix_stack<float, 4, 4>{
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f};
    }
    mat4 matrix_stack<float, 4, 4>::build_zero()
    {
        return matrix_stack<float, 4, 4>{
            0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f};
    }

    mat4 matrix_stack<float, 4, 4>::build_constant(float value)
    {
        return matrix_stack<float, 4, 4>{
            value, value, value, value,
            value, value, value, value,
            value, value, value, value,
            value, value, value, value};
    }

    int matrix_stack<float, 4, 4>::size() const { return 16; }
    int2 matrix_stack<float, 4, 4>::dimension() const { return { 4,4 }; }
    matrix_stack<float, 4, 4>& matrix_stack<float, 4, 4>::fill(float value)
    {
        auto it = begin();
        auto const it_end = end();
        for (; it != it_end; ++it)
            *it = value;
        return *this;
    }


    vec4 const& matrix_stack<float, 4, 4>::operator[](int k1) const {
        check_index_bounds(k1, *this);
        return at_unsafe(k1);
    }
    vec4& matrix_stack<float, 4, 4>::operator[](int k1) {
        check_index_bounds(k1, *this);
        return at_unsafe(k1);
    }

    vec4 const& matrix_stack<float, 4, 4>::operator()(int k1) const {
        check_index_bounds(k1, *this);
        return at_unsafe(k1);
    }
    vec4& matrix_stack<float, 4, 4>::operator()(int k1) {
        check_index_bounds(k1, *this);
        return at_unsafe(k1);
    }

    float const& matrix_stack<float, 4, 4>::operator()(int k1, int k2) const {
        check_index_bounds(k1, k2, *this);
        return at_unsafe(k1, k2);
    }
    float& matrix_stack<float, 4, 4>::operator()(int k1, int k2) {
        check_index_bounds(k1, k2, *this);
        return at_unsafe(k1, k2);
    }

    float const& matrix_stack<float, 4, 4>::at_offset(int offset) const {
        check_offset_bounds(offset, *this);
        return at_offset_unsafe(offset);
    }
    float& matrix_stack<float, 4, 4>::at_offset(int offset) {
        check_offset_bounds(offset, *this);
        return at_offset_unsafe(offset);
    }


    matrix_stack<float, 3, 3> matrix_stack<float, 4, 4>::remove_row_column(int idx1, int idx2) const
    {
        assert_cgp((idx1 < 4) && (idx2 < 4), "Incorrect index for removing row and column to matrix");
        matrix_stack<float, 3, 3> res;

        int k1_res = 0;
        for (int k1 = 0; k1 < 4; ++k1) {
            if (k1 != idx1) {
                int k2_res = 0;
                for (int k2 = 0; k2 < 4; ++k2) {
                    if (k2 != idx2) {
                        res.at(k1_res, k2_res) = at(k1, k2);
                        ++k2_res;
                    }
                }
                ++k1_res;
            }
        }

        return res;
    }






    float* matrix_stack<float, 4, 4>::begin() { return &at_unsafe(0, 0); }
    float* matrix_stack<float, 4, 4>::end() { return &at_unsafe(3, 3) + 1; }
    float const* matrix_stack<float, 4, 4>::begin() const { return &at_unsafe(0, 0); }
    float const* matrix_stack<float, 4, 4>::end() const { return &at_unsafe(3, 3) + 1; }
    float const* matrix_stack<float, 4, 4>::cbegin() const { return &at_unsafe(0, 0); }
    float const* matrix_stack<float, 4, 4>::cend() const { return &at_unsafe(3, 3) + 1; }


    float const& matrix_stack<float, 4, 4>::at_unsafe(int k1, int k2) const { return data.at_unsafe(k1).at_unsafe(k2); }
    float& matrix_stack<float, 4, 4>::at_unsafe(int k1, int k2) {
        return data.at_unsafe(k1).at_unsafe(k2);
    }

    vec4 const& matrix_stack<float, 4, 4>::at_unsafe(int k1) const { return data.at_unsafe(k1); }
    vec4& matrix_stack<float, 4, 4>::at_unsafe(int k1) { return data.at_unsafe(k1); }

    float const& matrix_stack<float, 4, 4>::at_offset_unsafe(int offset) const { return begin()[offset]; }
    float& matrix_stack<float, 4, 4>::at_offset_unsafe(int offset) { return begin()[offset]; }





    mat4 mat4::build_diagonal(float value)
    {
        return mat4(
            value, 0.0f, 0.0f, 0.0f,
            0.0f, value, 0.0f, 0.0f,
            0.0f, 0.0f, value, 0.0f,
            0.0f, 0.0f, 0.0f, value
            );
    }
    mat4 mat4::build_diagonal(vec4 const& arg)
    {
        matrix_stack<float, 4, 4> m;
        get<0, 0>(m) = arg.x;
        get<1, 1>(m) = arg.y;
        get<2, 2>(m) = arg.z;
        get<3, 3>(m) = arg.w;
        return m;
    }
    mat4 mat4::build_diagonal(float x, float y, float z, float w)
    {
        return mat4(
            x, 0, 0, 0,
            0, y, 0, 0,
            0, 0, z, 0,
            0, 0, 0, w);
    }


    mat4 mat4::build_scaling(float value)
    {
        return matrix_stack<float, 4, 4>(
            value, 0.0f, 0.0f, 0.0f,
            0.0f, value, 0.0f, 0.0f,
            0.0f, 0.0f, value, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
            );
    }

    mat4 mat4::build_scaling(vec3 const& arg)
    {
        return mat4(
            arg.x, 0.0f, 0.0f, 0.0f,
            0.0f, arg.y, 0.0f, 0.0f,
            0.0f, 0.0f, arg.z, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
            );
    }

    mat4 mat4::build_scaling(float x, float y, float z)
    {
        return mat4(
            x, 0.0f, 0.0f, 0.0f,
            0.0f, y, 0.0f, 0.0f,
            0.0f, 0.0f, z, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
            );
    }



    mat4 mat4::build_affine(mat3 const& linear, vec3 const& tr)
    {
        mat4 m(linear);
        m.set_translation(tr);
        return m;
    }


    mat4 mat4::build_translation(vec3 const& tr)
    {
        return mat4{
            1.0f, 0.0f, 0.0f, tr.x,
            0.0f, 1.0f, 0.0f, tr.y,
            0.0f, 0.0f, 1.0f, tr.z,
            0.0f, 0.0f, 0.0f, 1.0f};
    }

    mat4 mat4::build_translation(float x, float y, float z)
    {
        return mat4{
            1.0f, 0.0f, 0.0f, x,
            0.0f, 1.0f, 0.0f, y,
            0.0f, 0.0f, 1.0f, z,
            0.0f, 0.0f, 0.0f, 1.0f};
    }

    mat4 mat4::build_linear(mat3 const& M)
    {
        return mat4(M); // same than the constructor from mat3
    }

    mat4 mat4::build_rotation_from_axis_angle(vec3 const& axis, float angle)
    {
        // use the generation of matrix from rotation_transform structure
        return matrix_stack<float, 4, 4>(rotation_transform::from_axis_angle(axis, angle).matrix());
    }
    mat4 mat4::build_rotation_from_quaternion(quaternion const& q)
    {
        assert_cgp(cgp::abs(norm(q) - 1.0f) < 5e-2f, "Quaternion should have unit norm to represent rotation");
        return mat4(rotation_transform::from_quaternion(q).matrix());
    }
    mat4& mat4::set_linear(mat3 const& L)
    {
        for (int k1 = 0; k1 < 3; ++k1)
            for (int k2 = 0; k2 < 3; ++k2)
                at(k1, k2) = L.at(k1, k2);
        return *this;
    }
    mat4& mat4::set_rotation_from_axis_angle(vec3 const& axis, float angle)
    {
        set_linear(rotation_transform::from_axis_angle(axis, angle).matrix());
        return *this;
    }
    mat4& mat4::set_scaling(vec3 const& s)
    {
        return set_scaling(s.x, s.y, s.z);
    }
    mat4& mat4::set_scaling(float sx, float sy, float sz)
    {
        at(0, 0) = sx; at(0, 1) = 0; at(0, 2) = 0;
        at(1, 0) = 0; at(1, 1) = sy; at(1, 2) = 0;
        at(2, 0) = 0; at(2, 1) = 0; at(2, 2) = sz;
        return *this;
    }
    mat4& mat4::set_scaling(float s)
    {
        return set_scaling(s, s, s);
    }
    mat4& mat4::set_translation(vec3 const& tr)
    {
        data.x.w = tr.x;
        data.y.w = tr.y;
        data.z.w = tr.z;
        return *this;
    }
    mat4& mat4::set_translation(float x, float y, float z)
    {
        data.x.w = x;
        data.y.w = y;
        data.z.w = z;
        return *this;
    }


    vec3 mat4::get_translation() const
    {
        return vec3{ data.w.x, data.w.y, data.w.z };
    }
    mat3 mat4::get_linear() const
    {
        return mat3{
            data.x.xyz(),
            data.y.xyz(),
            data.z.xyz()
        };
    }



    vec3 mat4::apply_to_vec3_position(vec3 const& pos)
    {
        float const x = data.x.x * pos.x + data.x.y * pos.y + data.x.z * pos.z + data.x.w;
        float const y = data.y.x * pos.x + data.y.y * pos.y + data.y.z * pos.z + data.y.w;
        float const z = data.z.x * pos.x + data.z.y * pos.y + data.z.z * pos.z + data.z.w;
        float const w = data.w.x * pos.x + data.w.y * pos.y + data.w.z * pos.z + data.w.w;
        
        return vec3{ x / w, y / w, z / w };
    }


    vec3 mat4::apply_to_vec3_vector(vec3 const& v)
    {
        return {
            data.x.x * v.x + data.x.y * v.y + data.x.z * v.z,
            data.y.x * v.x + data.y.y * v.y + data.y.z * v.z,
            data.z.x * v.x + data.z.y * v.y + data.z.z * v.z
        };
    }


    vec4 mat4::col_x() const
    {
        return vec4(data.x.x, data.y.x, data.z.x, data.w.x);
    }
    vec4 mat4::col_y() const
    {
        return vec4(data.x.y, data.y.y, data.z.y, data.w.y);
    }
    vec4 mat4::col_z() const
    {
        return vec4(data.x.z, data.y.z, data.z.z, data.w.z);
    }
    vec4 mat4::col_w() const
    {
        return vec4(data.x.w, data.y.w, data.z.w, data.w.w);
    }
    vec4 const& mat4::row_x() const { return data.x; }
    vec4 const& mat4::row_y() const { return data.y; }
    vec4 const& mat4::row_z() const { return data.z; }
    vec4 const& mat4::row_w() const { return data.w; }
    vec4& mat4::row_x() { return data.x; }
    vec4& mat4::row_y() { return data.y; }
    vec4& mat4::row_z() { return data.z; }
    vec4& mat4::row_w() { return data.w; }

    vec3 mat4::col_x_vec3() const { return vec3(data.x.x, data.y.x, data.z.x); }
    vec3 mat4::col_y_vec3() const { return vec3(data.x.y, data.y.y, data.z.y); }
    vec3 mat4::col_z_vec3() const { return vec3(data.x.z, data.y.z, data.z.z); }
    vec3 mat4::col_w_vec3() const { return vec3(data.x.w, data.y.w, data.z.w); }
    vec3 mat4::row_x_vec3() const { return data.x.xyz(); }
    vec3 mat4::row_y_vec3() const { return data.y.xyz(); }
    vec3 mat4::row_z_vec3() const { return data.z.xyz(); }
    vec3 mat4::row_w_vec3() const { return data.w.xyz(); }



}