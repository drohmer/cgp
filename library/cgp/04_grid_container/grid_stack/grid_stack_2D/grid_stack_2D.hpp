#pragma once

#include "cgp/01_base/base.hpp"
#include "cgp/02_numarray/numarray_stack/numarray_stack.hpp"
#include "../../offset_grid/offset_grid.hpp"



/* ************************************************** */
/*           Header                                   */
/* ************************************************** */

namespace cgp
{

/** Container for 2D-grid like structure storing numerical element on stack (fixed size known at compile time)
 *
 * The grid_2D structure provide convenient access for 2D-grid organization where an element can be queried as grid_2D(i,j).
 * Elements of grid_2D are stored contiguously in stack memory and remain fully compatible with std::array and pointers.
 **/
template <typename T, int N1, int N2>
struct grid_stack_2D
{
    /** Internal storage as a 1D buffer */
    numarray_stack<T,N1*N2> data;

    /** Constructors */
    grid_stack_2D();
    grid_stack_2D(numarray_stack<T, N1*N2> const& elements);
    grid_stack_2D(std::initializer_list<T> const& arg);


    /** Total number of elements size = dimension[0] * dimension[1] */
    int size() const;
    /** Fill all elements of the grid_2D with the same element*/
    grid_stack_2D<T, N1, N2>& fill(T const& value);


    ///** Element access
    T const& operator[](int2 const& index) const; // grid_2D[ {x,y} ]
    T & operator[](int2 const& index);            // grid_2D[ {x,y} ]
    T const& operator()(int2 const& index) const; // grid_2D( {x,y} )
    T & operator()(int2 const& index);            // grid_2D( {x,y} )

    T const& operator()(int k1, int k2) const; // grid_2D(x, y) 
    T& operator()(int k1, int k2);            // grid_2D(x, y) 


    /** Iterators
    *  Iterators compatible with STL syntax and std::array */
    typename std::array<T, N1*N2>::iterator begin();
    typename std::array<T, N1* N2>::iterator end();
    typename std::array<T, N1* N2>::const_iterator begin() const;
    typename std::array<T, N1* N2>::const_iterator end() const;
    typename std::array<T, N1* N2>::const_iterator cbegin() const;
    typename std::array<T, N1* N2>::const_iterator cend() const;

};


template <typename T, int N1, int N2> std::string type_str(grid_stack_2D<T, N1, N2> const&);

/** Display all elements of the buffer.*/
template <typename T, int N1, int N2> std::ostream& operator<<(std::ostream& s, grid_stack_2D<T, N1, N2> const& v);


/** Direct compiled-checked access to data */
template <int idx1, int idx2, typename T, int N1, int N2> T const& element(grid_stack_2D<T, N1, N2> const& data);
template <int idx1, int idx2, typename T, int N1, int N2> T& element(grid_stack_2D<T, N1, N2>& data);


/** Convert all elements of the buffer to a string.
 * \param buffer: the input buffer
 * \param separator: the separator between each element
 */
template <typename T, int N1, int N2> std::string str(grid_stack_2D<T, N1, N2> const& v, std::string const& separator=" ", std::string const& begin = "", std::string const& end = "");


/** Equality test between grid_2D */
template <typename Ta, typename Tb, int Na1, int Na2, int Nb1, int Nb2> bool is_equal(grid_stack_2D<Ta,Na1,Na2> const& a, grid_stack_2D<Tb,Nb1,Nb2> const& b);

/** Math operators
 * Common mathematical operations between buffers, and scalar or element values. */
template <typename T, int N1, int N2> grid_stack_2D<T,N1,N2>& operator+=(grid_stack_2D<T, N1, N2>& a, grid_stack_2D<T,N1,N2> const& b);

template <typename T, int N1, int N2> grid_stack_2D<T, N1, N2>& operator+=(grid_stack_2D<T, N1, N2>& a, T const& b);
template <typename T, int N1, int N2> grid_stack_2D<T, N1, N2>  operator+(grid_stack_2D<T, N1, N2> const& a, grid_stack_2D<T,N1,N2> const& b);
template <typename T, int N1, int N2> grid_stack_2D<T, N1, N2>  operator+(grid_stack_2D<T, N1, N2> const& a, T const& b);
template <typename T, int N1, int N2> grid_stack_2D<T, N1, N2>  operator+(T const& a, grid_stack_2D<T, N1, N2> const& b);

template <typename T, int N1, int N2> grid_stack_2D<T, N1, N2>& operator-=(grid_stack_2D<T, N1, N2>& a, grid_stack_2D<T,N1,N2> const& b);
template <typename T, int N1, int N2> grid_stack_2D<T, N1, N2>& operator-=(grid_stack_2D<T, N1, N2>& a, T const& b);
template <typename T, int N1, int N2> grid_stack_2D<T, N1, N2>  operator-(grid_stack_2D<T, N1, N2> const& a, grid_stack_2D<T, N1, N2> const& b);
template <typename T, int N1, int N2> grid_stack_2D<T, N1, N2>  operator-(grid_stack_2D<T, N1, N2> const& a, T const& b);
template <typename T, int N1, int N2> grid_stack_2D<T, N1, N2>  operator-(T const& a, grid_stack_2D<T, N1, N2> const& b);

template <typename T, int N1, int N2> grid_stack_2D<T, N1, N2>& operator*=(grid_stack_2D<T, N1, N2>& a, grid_stack_2D<T, N1, N2> const& b);
template <typename T, int N1, int N2> grid_stack_2D<T, N1, N2>& operator*=(grid_stack_2D<T, N1, N2>& a, float b);
template <typename T, int N1, int N2> grid_stack_2D<T, N1, N2>  operator*(grid_stack_2D<T, N1, N2> const& a, grid_stack_2D<T, N1, N2> const& b);
template <typename T, int N1, int N2> grid_stack_2D<T, N1, N2>  operator*(grid_stack_2D<T, N1, N2> const& a, float b);
template <typename T, int N1, int N2> grid_stack_2D<T, N1, N2>  operator*(float a, grid_stack_2D<T, N1, N2> const& b);

template <typename T, int N1, int N2> grid_stack_2D<T, N1, N2>& operator/=(grid_stack_2D<T, N1, N2>& a, grid_stack_2D<T, N1, N2> const& b);
template <typename T, int N1, int N2> grid_stack_2D<T, N1, N2>& operator/=(grid_stack_2D<T, N1, N2>& a, float b);
template <typename T, int N1, int N2> grid_stack_2D<T, N1, N2>  operator/(grid_stack_2D<T, N1, N2> const& a, grid_stack_2D<T, N1, N2> const& b);
template <typename T, int N1, int N2> grid_stack_2D<T, N1, N2>  operator/(grid_stack_2D<T, N1, N2> const& a, float b);

// Matrix multiplication between two grid_stack_2D with compatible size
template <typename T, int N1, int N2, int N3> grid_stack_2D<T, N1, N3> multiply_matrix(grid_stack_2D<T, N1, N2> const& a, grid_stack_2D<T, N2, N3> const& b);

}



/* ************************************************** */
/*           IMPLEMENTATION                           */
/* ************************************************** */

namespace cgp
{


template <typename T, int N1, int N2>
grid_stack_2D<T,N1,N2>::grid_stack_2D()
    :data()
{}

template <typename T, int N1, int N2>
grid_stack_2D<T, N1, N2>::grid_stack_2D(numarray_stack<T, N1* N2> const& elements)
    : data(elements)
{}

template <typename T, int N1, int N2>
grid_stack_2D<T, N1, N2>::grid_stack_2D(std::initializer_list<T> const& arg)
    :data()
{
    assert_cgp(arg.size() >= N1 * N2, "Insufficient size to initialize grid_stack_2D");
    auto it = arg.begin();
    for (int k1 = 0; k1 < N1; ++k1) {
        for (int k2 = 0; k2 < N2; ++k2) {
            data[offset_grid_stack<N1>(k1, k2)] = *it;
            ++it;
        }
    }
}



template <typename T, int N1, int N2>
int grid_stack_2D<T,N1,N2>::size() const
{
    return N1*N2;
}


template <typename T, int N1, int N2>
grid_stack_2D<T, N1, N2>& grid_stack_2D<T,N1,N2>::fill(T const& value)
{
    data.fill(value);
    return *this;
}



template <typename T, int N1, int N2>
void check_index_bounds(int index1, int index2, grid_stack_2D<T,N1,N2> const& data)
{
#ifndef cgp_NO_DEBUG
    if (index1 < 0 || index2<0 || index1>=N1 || index2>=N2)
    {
        std::string msg = "\n";

        msg += "\t> Try to access grid_stack_2D(" + str(index1) + "," + str(index2) + ")\n";
        msg += "\t>    - grid_stack_2D has dimension = (" + str(N1) + "," + str(N2) + ")\n";
        msg += "\t>    - Type of grid_stack_2D: "+type_str(data)+"\n";

        if (index1<0 || index2<0)
        {
            msg += "\t> Buffer cannot be access with negative index.\n";
        }
        else if (N1==0 || N2==0)
        {
            msg += "\t> The buffer is empty, its elements cannot be accessed.\n";
        }
        else if (index1==N1 || index2==N2)
        {
            msg += "\t> Index reached the maximal size of the buffer \n";
            msg += "\t> The maximal possible indexes should be (" + str(N1-1) + ","+ str(N2 - 1) +") \n";
        }
        else if (index1>=N1 || index2>=N2)
        {
            msg += "\t> Exceeded buffer dimension \n";
        }


        msg += "\n\t  The function and variable that generated this error can be found in analysis the Call Stack.\n";

        error_cgp(msg);
    }
#endif
}



template <typename T, int N1, int N2>
T const& grid_stack_2D<T,N1,N2>::operator[](int2 const& index) const
{
    check_index_bounds(index.x, index.y, *this);
    int idx = offset_grid_stack<N1>(index.x, index.y);
    return data[idx];
}

template <typename T, int N1, int N2>
T& grid_stack_2D<T, N1, N2>::operator[](int2 const& index)
{
    check_index_bounds(index.x, index.y, *this);
    int idx = offset_grid_stack<N1>(index.x, index.y);
    return data[idx];
}
template <typename T, int N1, int N2>
T const& grid_stack_2D<T, N1, N2>::operator()(int2 const& index) const
{
    check_index_bounds(index.x, index.y, *this);
    int idx = offset_grid_stack<N1>(index.x, index.y);
    return data[idx];
}

template <typename T, int N1, int N2>
T& grid_stack_2D<T, N1, N2>::operator()(int2 const& index)
{
    check_index_bounds(index.x, index.y, *this);
    int idx = offset_grid_stack<N1>(index.x, index.y);
    return data[idx];
}




template <typename T, int N1, int N2>
T const& grid_stack_2D<T, N1, N2>::operator()(int k1, int k2) const
{
    check_index_bounds(k1, k2, *this);
    int idx = offset_grid_stack<N1>(k1, k2);
    return data[idx];
}

template <typename T, int N1, int N2>
T& grid_stack_2D<T, N1, N2>::operator()(int k1, int k2)
{
    check_index_bounds(k1, k2, *this);
    int idx = offset_grid_stack<N1>(k1, k2);
    return data[idx];
}




template <typename T, int N1, int N2>
typename std::array<T,N1*N2>::iterator grid_stack_2D<T, N1, N2>::begin()
{
    return data.begin();
}

template <typename T, int N1, int N2>
typename std::array<T, N1* N2>::iterator grid_stack_2D<T, N1, N2>::end()
{
    return data.end();
}

template <typename T, int N1, int N2>
typename std::array<T, N1* N2>::const_iterator grid_stack_2D<T, N1, N2>::begin() const
{
    return data.begin();
}

template <typename T, int N1, int N2>
typename std::array<T, N1* N2>::const_iterator grid_stack_2D<T, N1, N2>::end() const
{
    return data.end();
}

template <typename T, int N1, int N2>
typename std::array<T, N1* N2>::const_iterator grid_stack_2D<T, N1, N2>::cbegin() const
{
    return data.cbegin();
}

template <typename T, int N1, int N2>
typename std::array<T, N1* N2>::const_iterator grid_stack_2D<T, N1, N2>::cend() const
{
    return data.cend();
}










template <typename T, int N1, int N2> std::string type_str(grid_stack_2D<T, N1, N2> const&)
{
    return "grid_stack_2D<" + type_str(T()) +","+str(N1)+","+str(N2)+ ">";
}


template <typename Ta, typename Tb, int Na1, int Na2, int Nb1, int Nb2> bool is_equal(grid_stack_2D<Ta, Na1, Na2> const& a, grid_stack_2D<Tb, Nb1, Nb2> const& b)
{
    if (Na1!=Nb1 || Na2!=Nb2)
        return false;
    return is_equal(a.data, b.data);
}


template <int idx1, int idx2, typename T, int N1, int N2> T const& element(grid_stack_2D<T, N1, N2> const& data)
{
    static_assert(idx1 < N1 && idx2 < N2, "Index too large for grid_stack_2D access");
    return data.data.data[idx1 + N1 * idx2];
}
template <int idx1, int idx2, typename T, int N1, int N2> T& element(grid_stack_2D<T, N1, N2>& data)
{
    static_assert(idx1 < N1&& idx2 < N2, "Index too large for grid_stack_2D access");
    return data.data.data[idx1 + N1 * idx2];
}



template <typename T, int N1, int N2> std::ostream& operator<<(std::ostream& s, grid_stack_2D<T, N1, N2> const& v)
{
    return s << v.data;
}
template <typename T, int N1, int N2> std::string str(grid_stack_2D<T, N1, N2> const& v, std::string const& separator, std::string const& begin, std::string const& end)
{
    return str(v.data, separator, begin, end);
}


template <typename T, int N1, int N2> grid_stack_2D<T, N1, N2>& operator+=(grid_stack_2D<T, N1, N2>& a, grid_stack_2D<T, N1, N2> const& b)
{
    a.data += b.data;
    return a;
}
template <typename T, int N1, int N2> grid_stack_2D<T, N1, N2>& operator+=(grid_stack_2D<T, N1, N2>& a, T const& b)
{
    a.data += b;
    return a;
}
template <typename T, int N1, int N2> grid_stack_2D<T, N1, N2>  operator+(grid_stack_2D<T, N1, N2> const& a, grid_stack_2D<T, N1, N2> const& b)
{
    grid_stack_2D<T, N1, N2> res;
    res.data = a.data+b.data;
    return res;

}
template <typename T, int N1, int N2> grid_stack_2D<T, N1, N2> operator+(grid_stack_2D<T, N1, N2> const& a, T const& b)
{
    grid_stack_2D<T, N1, N2> res;
    res.data = a.data+b;
    return res;
}
template <typename T, int N1, int N2> grid_stack_2D<T, N1, N2>  operator+(T const& a, grid_stack_2D<T, N1, N2> const& b)
{
    grid_stack_2D<T, N1, N2> res;
    res.data = a + b.data;
    return res;
}

template <typename T, int N1, int N2> grid_stack_2D<T, N1, N2>& operator-=(grid_stack_2D<T, N1, N2>& a, grid_stack_2D<T, N1, N2> const& b)
{
    a.data += b.data;
    return a;
}
template <typename T, int N1, int N2> grid_stack_2D<T, N1, N2>& operator-=(grid_stack_2D<T, N1, N2>& a, T const& b)
{
    a.data -= b;
    return a;
}
template <typename T, int N1, int N2> grid_stack_2D<T, N1, N2>  operator-(grid_stack_2D<T, N1, N2> const& a, grid_stack_2D<T, N1, N2> const& b)
{
    grid_stack_2D<T, N1, N2> res;
    res.data = a.data-b.data;
    return res;
}
template <typename T, int N1, int N2> grid_stack_2D<T, N1, N2> operator-(grid_stack_2D<T, N1, N2> const& a, T const& b)
{
    grid_stack_2D<T, N1, N2> res;
    res.data = a.data-b;
    return res;
}
template <typename T, int N1, int N2> grid_stack_2D<T, N1, N2>  operator-(T const& a, grid_stack_2D<T, N1, N2> const& b)
{
    grid_stack_2D<T, N1, N2> res;
    res.data = a-b.data;
    return res;
}

template <typename T, int N1, int N2> grid_stack_2D<T, N1, N2>& operator*=(grid_stack_2D<T, N1, N2>& a, grid_stack_2D<T, N1, N2> const& b)
{
    a.data *= b.data;
    return a;
}
template <typename T, int N1, int N2> grid_stack_2D<T, N1, N2>& operator*=(grid_stack_2D<T, N1, N2>& a, float b)
{
    a.data *= b;
    return a;
}
template <typename T, int N1, int N2> grid_stack_2D<T, N1, N2>  operator*(grid_stack_2D<T, N1, N2> const& a, grid_stack_2D<T, N1, N2> const& b)
{
    grid_stack_2D<T, N1, N2> res;
    res.data = a.data*b.data;
    return res;
}
template <typename T, int N1, int N2> grid_stack_2D<T, N1, N2> operator*(grid_stack_2D<T, N1, N2> const& a, float b)
{
    grid_stack_2D<T, N1, N2> res;
    res.data = a.data*b;
    return res;
}
template <typename T, int N1, int N2> grid_stack_2D<T, N1, N2> operator*(float a, grid_stack_2D<T, N1, N2> const& b)
{
    grid_stack_2D<T, N1, N2> res;
    res.data = a*b.data;
    return res;
}

template <typename T, int N1, int N2> grid_stack_2D<T, N1, N2>& operator/=(grid_stack_2D<T, N1, N2>& a, grid_stack_2D<T, N1, N2> const& b)
{
    a.data /= b.data;
    return a;
}
template <typename T, int N1, int N2> grid_stack_2D<T, N1, N2>& operator/=(grid_stack_2D<T, N1, N2>& a, float b)
{
    a.data /= b;
    return a;
}
template <typename T, int N1, int N2> grid_stack_2D<T, N1, N2>  operator/(grid_stack_2D<T, N1, N2> const& a, grid_stack_2D<T, N1, N2> const& b)
{
    grid_stack_2D<T, N1, N2> res;
    res.data = a.data/b.data;
    return res;
}
template <typename T, int N1, int N2> grid_stack_2D<T, N1, N2>  operator/(grid_stack_2D<T, N1, N2> const& a, float b)
{
    grid_stack_2D<T, N1, N2> res;
    res.data = a.data/b;
    return res;
}

template <typename T, int N1, int N2, int N3> grid_stack_2D<T, N1, N3> multiply_matrix(grid_stack_2D<T, N1, N2> const& a, grid_stack_2D<T, N2, N3> const& b)
{
    grid_stack_2D<T, N1, N3> res;
    auto const& va = a.data.data;
    auto const& vb = b.data.data;
    for (int k1 = 0; k1 < N1; ++k1)
    {
        for (int k3 = 0; k3 < N3; ++k3)
        {
            T s{};
            for (int k2 = 0; k2 < N2; ++k2) {
                int const ka = offset_grid_stack<N1>(k1, k2);
                int const kb = offset_grid_stack<N2>(k2, k3);
                s += va[ka] * vb[kb]; // identical to s += a(k1,k2) * b(k2,k3)
            }

            res(k1, k3) = s;
        }
    }

    return res;
}

}
