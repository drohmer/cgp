#pragma once

#include "cgp/01_base/base.hpp"
#include "cgp/02_numarray/numarray.hpp"
#include "../../offset_grid/offset_grid.hpp"



/* ************************************************** */
/*           Header                                   */
/* ************************************************** */

namespace cgp
{

/** Container for 2D-grid like structure storing numerical element
 *
 * The grid_2D structure provide convenient access for 2D-grid organization where an element can be queried as grid_2D(i,j).
 * The indexing is obtained as grid_2D(k1,k2) = k1 + N1*k2
 * Elements of grid_2D are stored contiguously in heap memory and remain fully compatible with std::vector and pointers.
 **/
template <typename T>
struct grid_2D
{
    /** 2D dimension (Nx,Ny) of the container */
    int2 dimension;
    /** Internal storage as a 1D buffer */
    numarray<T> data;

    /** Constructors */
    grid_2D();                        // Empty buffer - no elements
    grid_2D(int size);                // Build a grid_2D of squared dimension (size,size)
    grid_2D(int2 const& size);        // Build a grid_2D with specified dimension
    grid_2D(int size_1, int size_2);  // Build a grid_2D with specified dimension

    /** Direct build a grid_2D from a given 1D-buffer and its 2D-dimension
    * \note: the size of the 1D-buffer must satisfy arg.size = size_1 * size_2 */
    static grid_2D<T> from_buffer(numarray<T> const& arg, int size_1, int size_2);


    /** Remove all elements from the grid_2D */
    void clear();
    /** Total number of elements size = dimension[0] * dimension[1] */
    int size() const;
    /** Fill all elements of the grid_2D with the same element*/
    void fill(T const& value);

    /** Resizing grid_2D */
    void resize(int size);         // Create a squared grid_2D of dimension (size,size) 
    void resize(int2 const& size);
    void resize(int size_1, int size_2);

    /** Element access
     * Bound checking is performed unless CGP_NO_DEBUG is defined. */

    T const& operator[](int2 const& index) const; // grid_2D[ {x,y} ]
    T & operator[](int2 const& index);            // grid_2D[ {x,y} ]
    T const& operator()(int2 const& index) const; // grid_2D( {x,y} )
    T & operator()(int2 const& index);            // grid_2D( {x,y} )

    T const& operator()(int k1, int k2) const; // grid_2D(x, y) 
    T& operator()(int k1, int k2);             // grid_2D(x, y) 


    int index_to_offset(int k1, int k2) const;
    int2 offset_to_index(int offset) const;

    /** Iterators
     * 1D-type iterators on grid_2D are compatible with STL syntax
     * allows "forall" loops (for(auto& e : buffer) {...}) */
    typename std::vector<T>::iterator begin();
    typename std::vector<T>::iterator end();
    typename std::vector<T>::const_iterator begin() const;
    typename std::vector<T>::const_iterator end() const;
    typename std::vector<T>::const_iterator cbegin() const;
    typename std::vector<T>::const_iterator cend() const;

    /** Direct access to the value - doesn't check index bounds*/
    inline T const& at(int index) const { return data.at(index); }
    inline T& at(int index) { return data.at(index); }


};


template <typename T> std::string type_str(grid_2D<T> const&);

/** Display all elements of the buffer.*/
template <typename T> std::ostream& operator<<(std::ostream& s, grid_2D<T> const& v);

/** Convert all elements of the buffer to a string.
 * \param buffer: the input buffer
 * \param separator: the separator between each element
 */
template <typename T> std::string str(grid_2D<T> const& v, std::string const& separator=" ", std::string const& begin = "", std::string const& end = "");


/** Equality test between grid_2D */
template <typename T1, typename T2> bool is_equal(grid_2D<T1> const& a, grid_2D<T2> const& b);

/** Math operators
 * Common mathematical operations between buffers, and scalar or element values. */
template <typename T> grid_2D<T>& operator+=(grid_2D<T>& a, grid_2D<T> const& b);

template <typename T> grid_2D<T>& operator+=(grid_2D<T>& a, T const& b);
template <typename T> grid_2D<T>  operator+(grid_2D<T> const& a, grid_2D<T> const& b);
template <typename T> grid_2D<T>  operator+(grid_2D<T> const& a, T const& b);
template <typename T> grid_2D<T>  operator+(T const& a, grid_2D<T> const& b);

template <typename T> grid_2D<T>& operator-=(grid_2D<T>& a, grid_2D<T> const& b);
template <typename T> grid_2D<T>& operator-=(grid_2D<T>& a, T const& b);
template <typename T> grid_2D<T>  operator-(grid_2D<T> const& a, grid_2D<T> const& b);
template <typename T> grid_2D<T>  operator-(grid_2D<T> const& a, T const& b);
template <typename T> grid_2D<T>  operator-(T const& a, grid_2D<T> const& b);

template <typename T> grid_2D<T>& operator*=(grid_2D<T>& a, grid_2D<T> const& b);
template <typename T> grid_2D<T>& operator*=(grid_2D<T>& a, float b);
template <typename T> grid_2D<T>  operator*(grid_2D<T> const& a, grid_2D<T> const& b);
template <typename T> grid_2D<T>  operator*(grid_2D<T> const& a, float b);
template <typename T> grid_2D<T>  operator*(float a, grid_2D<T> const& b);

template <typename T> grid_2D<T>& operator/=(grid_2D<T>& a, grid_2D<T> const& b);
template <typename T> grid_2D<T>& operator/=(grid_2D<T>& a, float b);
template <typename T> grid_2D<T>  operator/(grid_2D<T> const& a, grid_2D<T> const& b);
template <typename T> grid_2D<T>  operator/(grid_2D<T> const& a, float b);



}



/* ************************************************** */
/*           IMPLEMENTATION                           */
/* ************************************************** */

namespace cgp
{



template <typename T>
grid_2D<T>::grid_2D()
    :dimension(int2{0,0}),data()
{}

template <typename T>
grid_2D<T>::grid_2D(int size)
    :dimension({size,size}),data(size*size)
{
    assert_cgp_no_msg(size>0);
}

template <typename T>
grid_2D<T>::grid_2D(int2 const& size)
    :dimension(size),data(size[0]*size[1])
{
    assert_cgp_no_msg(size[0]>=0 && size[1]>=0);
}

template <typename T>
grid_2D<T>::grid_2D(int size_1, int size_2)
    :dimension({size_1,size_2}),data(size_1*size_2)
{
    assert_cgp_no_msg(size_1>=0 && size_2>=0);
}



template <typename T>
int grid_2D<T>::size() const
{
    return dimension[0]*dimension[1];
}

template <typename T>
void grid_2D<T>::clear()
{
    resize(0, 0);
}

template <typename T>
void grid_2D<T>::resize(int size)
{
    assert_cgp_no_msg(size>=0);
    resize(size,size);
}

template <typename T>
void grid_2D<T>::resize(int2 const& size)
{
    assert_cgp_no_msg(size[0]>=0 && size[1]>=0);
    dimension = size;
    data.resize(size[0]*size[1]);
}

template <typename T>
void grid_2D<T>::resize(int size_1, int size_2)
{
    assert_cgp_no_msg(size_1>=0 && size_2>=0);
    dimension = {size_1,size_2};
    resize({size_1,size_2});
}

template <typename T>
void grid_2D<T>::fill(T const& value)
{
    data.fill(value);
}


#ifndef CGP_NO_DEBUG
template <typename T>
void check_index_bounds(int index1, int index2, grid_2D<T> const& data)
{
    size_t const N1 = data.dimension.x;
    size_t const N2 = data.dimension.y;
    assert_cgp_no_msg(N1>=0 && N2>=0);
    if (index1 < 0 || index2<0 || index1>=N1 || index2>=N2)
    {
        std::string msg = "\n";

        msg += "\t> Try to access grid_2D(" + str(index1) + "," + str(index2) + ")\n";
        msg += "\t>    - grid_2D has dimension = (" + str(N1) + "," + str(N2) + ")\n";
        msg += "\t>    - Type of grid_2D: "+type_str(data)+"\n";

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
}
#else
template <typename T>
void check_index_bounds(int , int , grid_2D<T> const& ) {}
#endif



template <typename T>
T const& grid_2D<T>::operator[](int2 const& index) const
{
    check_index_bounds(index.x, index.y, *this);
    int const idx = offset_grid(index.x, index.y, dimension.x);
    return data[idx];
}

template <typename T>
T& grid_2D<T>::operator[](int2 const& index)
{
    check_index_bounds(index.x, index.y, *this);
    int const idx = offset_grid(index.x, index.y, dimension.x);

    return data[idx];
}

template <typename T>
T const& grid_2D<T>::operator()(int2 const& index) const
{
    return (*this)[index];
}

template <typename T>
T& grid_2D<T>::operator()(int2 const& index)
{
    return (*this)[index];
}


template <typename T>
T const& grid_2D<T>::operator()(int k1, int k2) const
{
    check_index_bounds(k1, k2, *this);
    int const idx = offset_grid(k1, k2, dimension.x);

    return data[idx];
}

template <typename T>
T& grid_2D<T>::operator()(int k1, int k2)
{
    check_index_bounds(k1, k2, *this);
    int const idx = offset_grid(k1, k2, dimension.x);

    return data[idx];
}




template <typename T>
typename std::vector<T>::iterator grid_2D<T>::begin()
{
    return data.begin();
}

template <typename T>
typename std::vector<T>::iterator grid_2D<T>::end()
{
    return data.end();
}

template <typename T>
typename std::vector<T>::const_iterator grid_2D<T>::begin() const
{
    return data.begin();
}

template <typename T>
typename std::vector<T>::const_iterator grid_2D<T>::end() const
{
    return data.end();
}

template <typename T>
typename std::vector<T>::const_iterator grid_2D<T>::cbegin() const
{
    return data.cbegin();
}

template <typename T>
typename std::vector<T>::const_iterator grid_2D<T>::cend() const
{
    return data.cend();
}










template <typename T> std::string type_str(grid_2D<T> const&)
{
    return "grid_2D<" + type_str(T()) + ">";
}


template <typename T1, typename T2> bool is_equal(grid_2D<T1> const& a, grid_2D<T2> const& b)
{
    if (is_equal(a.dimension, b.dimension)==false)
        return false;
    return is_equal(a.data, b.data);
}




template <typename T> std::ostream& operator<<(std::ostream& s, grid_2D<T> const& v)
{
    return s << v.data;
}
template <typename T> std::string str(grid_2D<T> const& v, std::string const& separator, std::string const& begin, std::string const& end)
{
    return to_string(v.data, separator, begin, end);
}


template <typename T> grid_2D<T>& operator+=(grid_2D<T>& a, grid_2D<T> const& b)
{
    assert_cgp( is_equal(a.dimension,b.dimension), "Dimension do not agree: a:"+str(a.dimension)+", b:"+str(b.dimension) );
    a.data += b.data;
}
template <typename T> grid_2D<T>& operator+=(grid_2D<T>& a, T const& b)
{
    a.data += b;
    return a;
}
template <typename T> grid_2D<T>  operator+(grid_2D<T> const& a, grid_2D<T> const& b)
{
    assert_cgp( is_equal(a.dimension,b.dimension), "Dimension do not agree: a:"+str(a.dimension)+", b:"+str(b.dimension) );
    grid_2D<T> res(a.dimension);
    res.data = a.data+b.data;
    return res;

}
template <typename T> grid_2D<T>  operator+(grid_2D<T> const& a, T const& b)
{
    grid_2D<T> res(a.dimension);
    res.data = a.data+b;
    return res;
}
template <typename T> grid_2D<T>  operator+(T const& a, grid_2D<T> const& b)
{
    grid_2D<T> res(b.dimension);
    res.data = a + b.data;
    return res;
}

template <typename T> grid_2D<T>& operator-=(grid_2D<T>& a, grid_2D<T> const& b)
{
    assert_cgp( is_equal(a.dimension,b.dimension), "Dimension do not agree: a:"+str(a.dimension)+", b:"+str(b.dimension) );
    a.data -= b.data;
}
template <typename T> grid_2D<T>& operator-=(grid_2D<T>& a, T const& b)
{
    a.data -= b;
    return a;
}
template <typename T> grid_2D<T>  operator-(grid_2D<T> const& a, grid_2D<T> const& b)
{
    assert_cgp( is_equal(a.dimension,b.dimension), "Dimension do not agree: a:"+str(a.dimension)+", b:"+str(b.dimension) );
    grid_2D<T> res(a.dimension);
    res.data = a.data-b.data;
    return res;
}
template <typename T> grid_2D<T>  operator-(grid_2D<T> const& a, T const& b)
{
    grid_2D<T> res(a.dimension);
    res.data = a.data-b;
    return res;
}
template <typename T> grid_2D<T>  operator-(T const& a, grid_2D<T> const& b)
{
    grid_2D<T> res(a.dimension);
    res.data = a-b.data;
    return res;
}

template <typename T> grid_2D<T>& operator*=(grid_2D<T>& a, grid_2D<T> const& b)
{
    assert_cgp( is_equal(a.dimension,b.dimension), "Dimension do not agree: a:"+str(a.dimension)+", b:"+str(b.dimension) );
    a.data *= b.data;
}
template <typename T> grid_2D<T>& operator*=(grid_2D<T>& a, float b)
{
    a.data *= b;
    return a;
}
template <typename T> grid_2D<T>  operator*(grid_2D<T> const& a, grid_2D<T> const& b)
{
    assert_cgp( is_equal(a.dimension,b.dimension), "Dimension do not agree: a:"+str(a.dimension)+", b:"+str(b.dimension) );
    grid_2D<T> res(a.dimension);
    res.data = a.data*b.data;
    return res;
}
template <typename T> grid_2D<T>  operator*(grid_2D<T> const& a, float b)
{
    grid_2D<T> res(a.dimension);
    res.data = a.data*b;
    return res;
}
template <typename T> grid_2D<T>  operator*(float a, grid_2D<T> const& b)
{
    grid_2D<T> res(b.dimension);
    res.data = a*b.data;
    return res;
}

template <typename T> grid_2D<T>& operator/=(grid_2D<T>& a, grid_2D<T> const& b)
{
    assert_cgp( is_equal(a.dimension,b.dimension), "Dimension do not agree: a:"+str(a.dimension)+", b:"+str(b.dimension) );
    a.data /= b.data;
    return a;
}
template <typename T> grid_2D<T>& operator/=(grid_2D<T>& a, float b)
{
    a.data /= b;
    return a;
}
template <typename T> grid_2D<T>  operator/(grid_2D<T> const& a, grid_2D<T> const& b)
{
    assert_cgp( is_equal(a.dimension,b.dimension), "Dimension do not agree: a:"+str(a.dimension)+", b:"+str(b.dimension) );
    grid_2D<T> res(a.dimension);
    res.data = a.data/b.data;
    return res;
}
template <typename T> grid_2D<T>  operator/(grid_2D<T> const& a, float b)
{
    grid_2D<T> res(a.dimension);
    res.data = a.data/b;
    return res;
}


template <typename T>
grid_2D<T> grid_2D<T>::from_buffer(numarray<T> const& arg, int size_1, int size_2)
{
    assert_cgp(arg.size()==size_1*size_2, "Incoherent size to generate grid_2D");

    grid_2D<T> b(size_1, size_2);
    b.data = arg;

    return b;
}

template <typename T>
int grid_2D<T>::index_to_offset(int k1, int k2) const
{
    return offset_grid(k1,k2,dimension.x);
}
template <typename T>
int2 grid_2D<T>::offset_to_index(int offset) const
{
    int2 idx = index_grid_from_offset(offset,dimension.x);
    return {idx.x, idx.y};
}


}
