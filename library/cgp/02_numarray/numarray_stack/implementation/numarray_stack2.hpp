#pragma once

#include "numarray_stack.hpp"

namespace cgp
{
    // Special case of a numarray_stack with dimension = 2
    //  Internal element can be accessed as x and y
    template <typename T>
    struct numarray_stack<T, 2>
    {
        /** Internal data */
        T x, y;


        numarray_stack<T, 2>();
        numarray_stack<T, 2>(T const& x, T const& y);
        template<typename T1,typename T2>
        numarray_stack<T, 2>(T1 const& x, T2 const& y);
        numarray_stack<T, 2>(numarray_stack<T,3> const& v);
        numarray_stack<T, 2>(numarray_stack<T,4> const& v);


    
        /** Size of the buffer = 2 */
        int size() const;

        /** Fill all data with the given value */
        numarray_stack<T, 2>& fill(T const& value);

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

    };
}



namespace cgp
{

    template <typename T>  numarray_stack<T, 2>::numarray_stack()
        :x(T()),y(T())
    {}

    template <typename T>  numarray_stack<T, 2>::numarray_stack(T const& x_arg, T const& y_arg)
        :x(x_arg),y(y_arg)
    {}

    template <typename T>
    template <typename T1,typename T2>
    numarray_stack<T, 2>::numarray_stack(T1 const& x_arg, T2 const& y_arg)
        :x(x_arg),y(y_arg)
    {}

    template <typename T>
    numarray_stack<T, 2>::numarray_stack(numarray_stack<T, 3> const& v)
        : x(v.x), y(v.y)
    {}

    template <typename T>
    numarray_stack<T, 2>::numarray_stack(numarray_stack<T, 4> const& v)
        : x(v.x), y(v.y)
    {}
    
    template <typename T> int numarray_stack<T, 2>::size() const
    {
        return 2;
    }



    template <typename T>  numarray_stack<T, 2>& numarray_stack<T, 2>::fill(T const& value)
    {
        x = value;
        y = value;
        return *this;
    }

    template <typename T, int N, typename INDEX_TYPE>
    void check_index_bounds(INDEX_TYPE index, numarray_stack<T, 2> const& data)
    {
#ifndef cgp_NO_DEBUG
        if (index < 0 || index>1)
        {
            std::string msg = "\n";
            msg += " - Try access "+type_str(data)+"[" + str(index) + "]\n";
            msg += " - Indexing is limited to 0 or 1";
            error_cgp(msg);
        }
#endif
    }

    template <typename T> T const& numarray_stack<T, 2>::operator[](int index) const
    {
        check_index_bounds(index, *this);
        return (&x)[index];
    }

    template <typename T> T& numarray_stack<T, 2>::operator[](int index)
    {
        check_index_bounds(index, *this);
        return (&x)[index];
    }

    template <typename T> T const& numarray_stack<T, 2>::operator()(int index) const
    {
        check_index_bounds(index, *this);
        return (&x)[index];
    }
    template <typename T> T& numarray_stack<T, 2>::operator()(int index)
    {
        check_index_bounds(index, *this);
        return (&x)[index];
    }



    template <typename T> T const& numarray_stack<T, 2>::at(int index) const
    {
        check_index_bounds(index, *this);
        return (&x)[index];
    }
    template <typename T> T& numarray_stack<T, 2>::at(int index)
    {
        check_index_bounds(index, *this);
        return (&x)[index];
    }


    template <typename T> T* numarray_stack<T, 2>::begin() { return &x; }
    template <typename T> T* numarray_stack<T, 2>::end() { return &y+1; }
    template <typename T> T const* numarray_stack<T, 2>::begin() const { return &x; }
    template <typename T> T const* numarray_stack<T, 2>::end() const { return &y+1; }
    template <typename T> T const* numarray_stack<T, 2>::cbegin() const { return &x; }
    template <typename T> T const* numarray_stack<T, 2>::cend() const { return &y+1; }


    template <int idx, typename T> T const& get(numarray_stack<T, 2> const& data)
    {
        static_assert(idx < 2, "Incorrect element indexing");
        return (&data.x)[idx];
    }
    template <int idx, typename T> T& get(numarray_stack<T, 2>& data)
    {
        static_assert(idx < 2, "Incorrect element indexing");
        return (&data.x)[idx];
    }



    template <typename T> std::string type_str(numarray_stack<T, 2> const&)
    {
        return "numarray_stack2<" + type_str(T()) + ">";
    }
}