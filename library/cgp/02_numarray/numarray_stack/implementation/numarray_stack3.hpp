#pragma once

#include "numarray_stack.hpp"



namespace cgp
{
    // Special case of a numarray_stack with dimension = 3
    //  Internal element can be accessed as x, y and z
    template <typename T>
    struct numarray_stack<T, 3>
    {
        /** Internal data */
        T x, y, z;



        numarray_stack<T, 3>();
        numarray_stack<T, 3>(T const& x, T const& y, T const& z);
        numarray_stack<T, 3>(numarray_stack<T, 2> const& xy, T const& z);
        numarray_stack<T, 3>(T const& x, numarray_stack<T, 2> const& yz);
        template<typename T1,typename T2, typename T3>
        numarray_stack<T,3>(T1 const& x, T2 const& y, T3 const& z);



        /** Size of the buffer = 3 */
        int size() const;

        /** Fill all data with the given value */
        numarray_stack<T, 3>& fill(T const& value);

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
        numarray_stack<T,2> xy() const;
        numarray_stack<T,2> yz() const;
        numarray_stack<T,2> xz() const;
    };


}



namespace cgp
{

    template <typename T> numarray_stack<T, 3>::numarray_stack()
        :x(T()),y(T()),z(T())
    {}
    template <typename T> numarray_stack<T, 3>::numarray_stack(T const& x_arg, T const& y_arg, T const& z_arg)
        : x(x_arg), y(y_arg), z(z_arg)
    {}
    template <typename T> numarray_stack<T, 3>::numarray_stack(numarray_stack<T, 2> const& xy, T const& z_arg)
        : x(get<0>(xy)), y(get<1>(xy)), z(z_arg)
    {}
    template <typename T> numarray_stack<T, 3>::numarray_stack(T const& x_arg, numarray_stack<T, 2> const& yz)
        : x(x_arg), y(get<0>(yz)), z(get<1>(yz))
    {}

    template <typename T>
    template<typename T1,typename T2, typename T3>
    numarray_stack<T, 3>::numarray_stack(T1 const& x_arg, T2 const& y_arg, T3 const& z_arg)
        :x(T(x_arg)), y(T(y_arg)), z(T(z_arg))
    {}


    template <typename T> int numarray_stack<T, 3>::size() const
    {
        return 3;
    }



    template <typename T>  numarray_stack<T, 3>& numarray_stack<T, 3>::fill(T const& value)
    {
        x = value;
        y = value;
        z = value;
        return *this;
    }

#ifndef cgp_NO_DEBUG
    template <typename T, int N, typename INDEX_TYPE>
    void check_index_bounds(INDEX_TYPE index, numarray_stack<T, 3> const& data)
    {

        if (index < 0 || index>2)
        {
            std::string msg = "\n";
            msg += " - Try access " + type_str(data) + "[" + str(index) + "]\n";
            msg += " - Indexing is limited to 0, 1, 2";
            error_cgp(msg);
        }
    }
#else
    template <typename T, int N, typename INDEX_TYPE>
    void check_index_bounds(INDEX_TYPE, numarray_stack<T, 3> const&) {}
#endif

    template <typename T> T const& numarray_stack<T, 3>::operator[](int index) const
    {
        check_index_bounds(index, *this);
        return (&x)[index];
    }

    template <typename T> T& numarray_stack<T, 3>::operator[](int index)
    {
        check_index_bounds(index, *this);
        return (&x)[index];
    }

    template <typename T> T const& numarray_stack<T, 3>::operator()(int index) const
    {
        check_index_bounds(index, *this);
        return (&x)[index];
    }
    template <typename T> T& numarray_stack<T, 3>::operator()(int index)
    {
        check_index_bounds(index, *this);
        return (&x)[index];
    }


    template <typename T> T const& numarray_stack<T, 3>::at(int index) const
    {
        check_index_bounds(index, *this);
        return (&x)[index];
    }
    template <typename T> T& numarray_stack<T, 3>::at(int index)
    {
        check_index_bounds(index, *this);
        return (&x)[index];
    }


    template <typename T> T* numarray_stack<T, 3>::begin() { return &x; }
    template <typename T> T* numarray_stack<T, 3>::end() { return &z+1; }
    template <typename T> T const* numarray_stack<T, 3>::begin() const { return &x; }
    template <typename T> T const* numarray_stack<T, 3>::end() const { return &z+1; }
    template <typename T> T const* numarray_stack<T, 3>::cbegin() const { return &x; }
    template <typename T> T const* numarray_stack<T, 3>::cend() const { return &z+1; }





    template <typename T> numarray_stack<T,2> numarray_stack<T, 3>::xy() const
    {
        return numarray_stack<T, 2>{x, y};
    }
    template <typename T> numarray_stack<T,2> numarray_stack<T, 3>::yz() const
    {
        return numarray_stack<T, 2>{y, z};
    }
    template <typename T> numarray_stack<T,2> numarray_stack<T, 3>::xz() const
    {
        return numarray_stack<T, 2>{x, z};
    }


    template <int idx, typename T> T const& get(numarray_stack<T, 3> const& data)
    {
        static_assert(idx < 3, "Incorrect element indexing");
        return (&data.x)[idx];
    }
    template <int idx, typename T> T& get(numarray_stack<T, 3>& data)
    {
        static_assert(idx < 3, "Incorrect element indexing");
        return (&data.x)[idx];
    }


    template <typename T> std::string type_str(numarray_stack<T, 3> const&)
    {
        return "numarray_stack3<" + type_str(T()) + ">";
    }



}