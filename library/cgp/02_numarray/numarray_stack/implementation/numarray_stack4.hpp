#pragma once

#include "numarray_stack.hpp"

namespace cgp
{
    // Special case of a numarray_stack with dimension = 4
    //  Internal element can be accessed as x, y, z and w
    template <typename T>
    struct numarray_stack<T, 4>
    {
        /** Internal data */
        T x, y, z, w;


        numarray_stack<T, 4>();
        numarray_stack<T, 4>(T const& x, T const& y, T const& z, T const& w);
        numarray_stack<T, 4>(numarray_stack<T, 3> const& xyz, T const& w);
        numarray_stack<T, 4>(T const& x, numarray_stack<T, 3> const& yzw);

        numarray_stack<T, 4>(T const& x, T const& y, numarray_stack<T, 2> const& yz);
        numarray_stack<T, 4>(numarray_stack<T, 2> const& xy, T const& z, T const& w);
        numarray_stack<T, 4>(T const& x, numarray_stack<T, 2> const& yz, T const& w);
        numarray_stack<T, 4>(numarray_stack<T, 2> const& xy, numarray_stack<T, 2> const& zw);


        /** Size of the buffer = 4 */
        int size() const;

        /** Fill all data with the given value */
        numarray_stack<T, 4>& fill(T const& value);

        /** Element access
         * Allows buffer[i], buffer(i), and buffer.at(i)
         * Bound checking is performed unless cgp_NO_DEBUG is defined. */
        T const& operator[](int index) const;
        T& operator[](int index);

        T const& operator()(int index) const;
        T& operator()(int index);

        T const& at(int index) const;
        T& at(int index);


        /** Iterators
         *  Iterators compatible with STL syntax and std::array */
        T* begin();
        T* end();
        T const* begin() const;
        T const* end() const;
        T const* cbegin() const;
        T const* cend() const;

        T const& at_unsafe(int index) const { return (&x)[index]; }
        T& at_unsafe(int index) { return (&x)[index]; }

        /** Sub-vector */
        numarray_stack<T, 3> xyz() const;
        numarray_stack<T, 2> xy() const;
        numarray_stack<T, 2> yz() const;
        numarray_stack<T, 2> xz() const;
        
    };
}



namespace cgp
{

    template <typename T> numarray_stack<T, 4>::numarray_stack()
        :x(T()), y(T()), z(T()), w(T())
    {}
    template <typename T> numarray_stack<T, 4>::numarray_stack(T const& x_arg, T const& y_arg, T const& z_arg, T const& w_arg)
        : x(x_arg), y(y_arg), z(z_arg), w(w_arg)
    {}
    template <typename T> numarray_stack<T, 4>::numarray_stack(numarray_stack<T, 3> const& xyz, T const& w_arg)
        : x(get<0>(xyz)), y(get<1>(xyz)), z(get<2>(xyz)), w(w_arg)
    {}
    template <typename T> numarray_stack<T, 4>::numarray_stack(T const& x_arg, numarray_stack<T, 3> const& yzw)
        : x(x_arg), y(get<0>(yzw)), z(get<1>(yzw)), w(get<2>(yzw))
    {}

    template <typename T> numarray_stack<T, 4>::numarray_stack(T const& x_arg, T const& y_arg, numarray_stack<T, 2> const& yz)
        : x(x_arg), y(y_arg), z(get<0>(yz)), w(get<1>(yz))
    {}
    template <typename T> numarray_stack<T, 4>::numarray_stack(numarray_stack<T, 2> const& xy, T const& z_arg, T const& w_arg)
        : x(get<0>(xy)), y(get<1>(xy)), z(z_arg), w(w_arg)
    {}
    template <typename T> numarray_stack<T, 4>::numarray_stack(T const& x_arg, numarray_stack<T, 2> const& yz, T const& w_arg)
        : x(x_arg), y(get<0>(yz)), z(get<1>(yz)), w(w_arg)
    {}
    template <typename T> numarray_stack<T, 4>::numarray_stack(numarray_stack<T, 2> const& xy, numarray_stack<T, 2> const& zw)
        : x(get<0>(xy)), y(get<1>(xy)), z(get<0>(zw)), w(get<1>(zw))
    {}


    template <typename T> int numarray_stack<T, 4>::size() const
    {
        return 4;
    }



    template <typename T>  numarray_stack<T, 4>& numarray_stack<T, 4>::fill(T const& value)
    {
        x = value;
        y = value;
        z = value;
        w = value;
        return *this;
    }

    template <typename T, int N, typename INDEX_TYPE>
    void check_index_bounds(INDEX_TYPE index, numarray_stack<T, 4> const& data)
    {
#ifndef cgp_NO_DEBUG
        if (index < 0 || index>3)
        {
            std::string msg = "\n";
            msg += " - Try access " + type_str(data) + "[" + str(index) + "]\n";
            msg += " - Indexing is limited to 0, 1, 2, 3";
            error_cgp(msg);
        }
#endif
    }

    template <typename T> T const& numarray_stack<T, 4>::operator[](int index) const
    {
        check_index_bounds(index, *this);
        return (&x)[index];
    }

    template <typename T> T& numarray_stack<T, 4>::operator[](int index)
    {
        check_index_bounds(index, *this);
        return (&x)[index];
    }

    template <typename T> T const& numarray_stack<T, 4>::operator()(int index) const
    {
        check_index_bounds(index, *this);
        return (&x)[index];
    }
    template <typename T> T& numarray_stack<T, 4>::operator()(int index)
    {
        check_index_bounds(index, *this);
        return (&x)[index];
    }


    template <typename T> T const& numarray_stack<T, 4>::at(int index) const
    {
        check_index_bounds(index, *this);
        return (&x)[index];
    }
    template <typename T> T& numarray_stack<T, 4>::at(int index)
    {
        check_index_bounds(index, *this);
        return (&x)[index];
    }


    template <typename T> T* numarray_stack<T, 4>::begin() { return &x; }
    template <typename T> T* numarray_stack<T, 4>::end() { return &w + 1; }
    template <typename T> T const* numarray_stack<T, 4>::begin() const { return &x; }
    template <typename T> T const* numarray_stack<T, 4>::end() const { return &w + 1; }
    template <typename T> T const* numarray_stack<T, 4>::cbegin() const { return &x; }
    template <typename T> T const* numarray_stack<T, 4>::cend() const { return &w + 1; }




    template <typename T> numarray_stack<T, 3> numarray_stack<T, 4>::xyz() const
    {
        return numarray_stack<T, 3>{x, y, z};
    }
    template <typename T> numarray_stack<T, 2> numarray_stack<T, 4>::xy() const
    {
        return numarray_stack<T, 2>{x, y};
    }
    template <typename T> numarray_stack<T, 2> numarray_stack<T, 4>::yz() const
    {
        return numarray_stack<T, 2>{y, z};
    }
    template <typename T> numarray_stack<T, 2> numarray_stack<T, 4>::xz() const
    {
        return numarray_stack<T, 2>{x, z};
    }


    template <int idx, typename T> T const& get(numarray_stack<T, 4> const& data)
    {
        static_assert(idx < 4, "Incorrect element indexing");
        return (&data.x)[idx];
    }
    template <int idx, typename T> T& get(numarray_stack<T, 4>& data)
    {
        static_assert(idx < 4, "Incorrect element indexing");
        return (&data.x)[idx];
    }

    template <typename T> std::string type_str(numarray_stack<T, 4> const&)
    {
        return "numarray_stack4<" + type_str(T()) + ">";
    }
}