#include "cgp/01_base/base.hpp"

#include "mat2.hpp"


namespace cgp
{
    mat2::matrix_stack()
        :data()
    {}
    mat2::matrix_stack(numarray_stack<vec2, 2> const& elements)
        : data(elements)
    {}
    mat2::matrix_stack(vec2 const& row_1, vec2 const& row_2)
        : data({ row_1,row_2 })
    {}
    mat2::matrix_stack(numarray_stack<float, 4> const& elements)
        : data({
            vec2{ get<0>(elements),get<1>(elements) },
            vec2{ get<2>(elements),get<3>(elements) } })
    {}
    mat2::matrix_stack(
        float xx, float xy,
        float yx, float yy)
        : data({ vec2{xx,xy},vec2{yx,yy} })
    {}


    mat2::matrix_stack(float value)
        :data({
        vec2(value,0),
        vec2(0,value) })
    {}
    mat2::matrix_stack(float xx, float yy)
        :data({
        vec2(xx,0),
        vec2(0,yy) })
    {}
    mat2::matrix_stack(std::initializer_list<float> const& arg)
        :data()
    {
        if(arg.size()>=4) {
            auto it_arg = arg.begin();

            data.x.x = *it_arg; ++it_arg; data.x.y = *it_arg; ++it_arg;
            data.y.x = *it_arg; ++it_arg; data.y.y = *it_arg;
        }
        else if(arg.size()==1) {
            *this=mat2(*arg.begin());
        }
        else if(arg.size()==2) {
            auto it_arg = arg.begin();
            float xx= *it_arg; it_arg++;
            float yy= *it_arg; 
            *this=mat2(xx, yy);
        }
        else {
            error_cgp("Incoherent size to initialize mat2 with initialize_list<float> ("+str(arg.size())+")");
        }

    }
    mat2::matrix_stack(std::initializer_list<vec2> const& arg)
        :data()
    {
        assert_cgp(arg.size() >= 2, "Insufficient size to initialize mat2 with initialize_list<vec2>");
        auto it_arg = arg.begin();

        data.x = *it_arg; ++it_arg;
        data.y = *it_arg;
    }


    vec2 const& mat2::operator[](int k2) const
    {
        check_index_bounds(k2, *this);
        return at(k2);
    }
    vec2& mat2::operator[](int k2)
    {
        check_index_bounds(k2, *this);
        return at(k2);
    }

    vec2 const& mat2::operator()(int k2) const
    {
        check_index_bounds(k2, *this);
        return at(k2);
    }
    vec2& mat2::operator()(int k2)
    {
        check_index_bounds(k2, *this);
        return at(k2);
    }

    float const& mat2::operator()(int k1, int k2) const
    {
        check_index_bounds(k1,k2, *this);
        return at(k1,k2);
    }
    float& mat2::operator()(int k1, int k2)
    {
        check_index_bounds(k1, k2, *this);
        return at(k1, k2);
    }



    int mat2::size() const { return 4; }
    int2 mat2::dimension() const { return { 2,2 }; }
    mat2& mat2::fill(float value) {
        data.x.fill(value);
        data.y.fill(value);
        return *this;
    }



    
    mat2 mat2::build_identity() {return mat2(1, 0, 0, 1); }
    mat2 mat2::build_constant(float value) { return mat2(value, value, value, value); }

    
    mat2 mat2::build_diagonal(float value) { return mat2(value, 0, 0, value); }
    mat2 mat2::build_diagonal(vec2 const& arg) { return mat2(arg.x, 0, 0, arg.y); }
    mat2 mat2::build_diagonal(float x, float y) { return mat2(x, 0, 0, y); }

    mat2 mat2::build_scaling(float value) { return build_diagonal(value); }
    mat2 mat2::build_scaling(vec2 const& arg) { return build_diagonal(arg); }
    mat2 mat2::build_scaling(float x, float y) { return build_diagonal(x,y); }

    mat2 mat2::build_rotation(float theta)
    {
        float const c = std::cos(theta);
        float const s = std::sin(theta);
        return mat2(
            c, -s, 
            s, c);
    }

}


