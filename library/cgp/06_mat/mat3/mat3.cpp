#include "cgp/01_base/base.hpp"

#include "mat3.hpp"
#include "cgp/09_geometric_transformation/rotation_transform/rotation_transform.hpp"


namespace cgp
{
    mat3::matrix_stack()
        :data()
    {}
    mat3::matrix_stack(numarray_stack< vec3, 3> const& elements)
        : data(elements)
    {}
    mat3::matrix_stack(vec3 const& row_1, vec3 const& row_2, vec3 const& row_3)
        : data({ row_1,row_2, row_3 })
    {}
    mat3::matrix_stack(numarray_stack<float, 9> const& elements)
        : data({
            vec3{ get<0>(elements),get<1>(elements),get<2>(elements)},
            vec3{ get<3>(elements),get<4>(elements),get<5>(elements)},
            vec3{ get<6>(elements),get<7>(elements),get<8>(elements)} })
    {}
    mat3::matrix_stack(
        float xx, float xy, float xz,
        float yx, float yy, float yz,
        float zx, float zy, float zz)
        : data({ vec3{xx,xy,xz},vec3{yx,yy,yz},vec3{zx,zy,zz} })
    {}

    mat3::matrix_stack(float value)         
        :data({
        vec3(value,0,0),
        vec3(0,value,0),
        vec3(0,0,value) })
    {}
    mat3::matrix_stack(float xx, float yy, float zz) 
        :data({
        vec3(xx,0,0),
        vec3(0,yy,0),
        vec3(0,0,zz) })
    {}

    mat3::matrix_stack(std::initializer_list<float> const& arg)
        :data()
    {
        if(arg.size()>=9) {
            auto it_arg = arg.begin();

            data.x.x = *it_arg; ++it_arg; data.x.y = *it_arg; ++it_arg; data.x.z = *it_arg; ++it_arg;
            data.y.x = *it_arg; ++it_arg; data.y.y = *it_arg; ++it_arg; data.y.z = *it_arg; ++it_arg;
            data.z.x = *it_arg; ++it_arg; data.z.y = *it_arg; ++it_arg; data.z.z = *it_arg;
        }
        else if(arg.size()==1) {
            *this=mat3(*arg.begin());
        }
        else if(arg.size()==3) {
            auto it_arg = arg.begin();
            float xx= *it_arg; it_arg++;
            float yy= *it_arg; it_arg++;
            float zz= *it_arg;
            *this=mat3(xx, yy, zz);
        }
        else {
            error_cgp("Incoherent size to initialize mat3 with initialize_list<float> ("+str(arg.size())+")");
        }

    }
    mat3::matrix_stack(std::initializer_list<vec3> const& arg)
        :data()
    {
        assert_cgp(arg.size() >= 3, "Insufficient size to initialize mat3 with initialize_list<vec3>");
        auto it_arg = arg.begin();

        data.x = *it_arg; ++it_arg;
        data.y = *it_arg; ++it_arg;
        data.z = *it_arg; 
    }


    vec3 const& mat3::operator[](int k2) const
    {
        check_index_bounds(k2, *this);
        return at(k2);
    }
    vec3& mat3::operator[](int k2)
    {
        check_index_bounds(k2, *this);
        return at(k2);
    }

    vec3 const& mat3::operator()(int k2) const
    {
        check_index_bounds(k2, *this);
        return at(k2);
    }
    vec3& mat3::operator()(int k2)
    {
        check_index_bounds(k2, *this);
        return at(k2);
    }

    float const& mat3::operator()(int k1, int k2) const
    {
        check_index_bounds(k1,k2, *this);
        return at(k1,k2);
    }
    float& mat3::operator()(int k1, int k2)
    {
        check_index_bounds(k1, k2, *this);
        return at(k1, k2);
    }



    int mat3::size() const { return 9; }
    int2 mat3::dimension() const { return { 3,3 }; }
    mat3& mat3::fill(float value) {
        data.x.fill(value);
        data.y.fill(value);
        data.z.fill(value);
        return *this;
    }



    
    mat3 mat3::build_identity() {return mat3(1,0,0, 0,1,0, 0,0,1); }
    mat3 mat3::build_zero() { return mat3(0,0,0, 0,0,0, 0,0,0); }
    mat3 mat3::build_constant(float value) { return mat3(value,value,value, value,value,value, value,value,value); }

    
    mat3 mat3::build_diagonal(float value) { return mat3(value, 0, 0, 0,value,0, 0,0,value); }
    mat3 mat3::build_diagonal(vec3 const& arg) { return mat3(arg.x, 0, 0, 0,arg.y,0, 0,arg.z,0); }
    mat3 mat3::build_diagonal(float x, float y, float z) { return mat3(x, 0, 0, 0,y,0, 0,0,z); }

    mat3 mat3::build_scaling(float value) { return build_diagonal(value); }
    mat3 mat3::build_scaling(vec3 const& arg) { return build_diagonal(arg); }
    mat3 mat3::build_scaling(float x, float y, float z) { return build_diagonal(x,y,z); }

    mat3 mat3::build_rotation_from_axis_angle(vec3 const& axis, float angle)
    {
        return rotation_transform::from_axis_angle(axis, angle).matrix();
    }
    mat3 mat3::build_rotation_from_quaternion(quaternion const& q)
    {
        assert_cgp(cgp::abs(norm(q) - 1.0f) < 5e-2f, "Quaternion should have unit norm to represent rotation");
        return rotation_transform::from_quaternion(q).matrix();
    }

}


