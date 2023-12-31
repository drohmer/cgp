#pragma once

#include "cgp/01_base/base.hpp"
#include "cgp/02_numarray/numarray.hpp"
#include "../../offset_grid/offset_grid.hpp"


/* ************************************************** */
/*           Header                                   */
/* ************************************************** */

namespace cgp
{

/** Container for 3D-grid like structure storing numerical element
*
* The grid_3D structure provide convenient access for 3D-grid organization where an element can be queried as grid_3D(i,j).
* Elements of grid_3D are stored contiguously in heap memory and remain fully compatible with std::vector and pointers.
**/
template <typename T>
struct grid_3D
{
    /** 3D dimension (Nx,Ny,Nz) of the container */
    int3 dimension;
    /** Internal storage as a 1D buffer */
    numarray<T> data;

    /** Constructors */
    grid_3D();                 // Emtpy grid
    grid_3D(int size);         // Generate a grid of dimension size x size x size
    grid_3D(int3 const& size); // Generate a grid of dimension size.x size.y size.z
    grid_3D(int size_1, int size_2, int size_3); // Generate a grid of dimension size_1 x size_2 x size_3

    /** Direct build a grid_3D from a given 1D-buffer and its 3D-dimension
    * \note: the size of the 3D-buffer must satisfy arg.size = size_1 * size_2 * size_3 */
    static grid_3D<T> from_array(numarray<T> const& arg, int size_1, int size_2, int size_3);

    /** Remove all elements from the grid_2D */
    void clear();
    /** Total number of elements size = dimension[0] * dimension[1] * dimension[2] */
    int size() const;
    /** Fill all elements of the grid_3D with the same element*/
    void fill(T const& value);

    /** Resizing grid_3D (do not necessarily clear the previous values) */
    void resize(int size);
    void resize(int3 const& size);
    void resize(int size_1, int size_2, int size_3);
    
    /** Element access
     * Bound checking is performed unless CGP_NO_DEBUG is defined. */
    T const& operator[](int3 const& index) const;
    T& operator[](int3 const& index);
    T const& operator()(int3 const& index) const;
    T& operator()(int3 const& index);

    T const& operator()(int k1, int k2, int k3) const;
    T& operator()(int k1, int k2, int k3);

    int index_to_offset(int k1, int k2, int k3) const;
    int index_to_offset(int3 const& index) const;
    int3 offset_to_index(int offset) const;

    typename std::vector<T>::iterator begin();
    typename std::vector<T>::iterator end();
    typename std::vector<T>::const_iterator begin() const;
    typename std::vector<T>::const_iterator end() const;
    typename std::vector<T>::const_iterator cbegin() const;
    typename std::vector<T>::const_iterator cend() const;

    T const& at_unsafe(int index) const;
    T & at_unsafe(int index);           
    T const& at_unsafe(int index1, int index2, int index3) const;
    T & at_unsafe(int index1, int index2, int index3);

};

template <typename T> std::string type_str(grid_3D<T> const&);
template <typename T1, typename T2> bool is_equal(grid_3D<T1> const& a, grid_3D<T2> const& b);

template <typename T> std::ostream& operator<<(std::ostream& s, grid_3D<T> const& v);
template <typename T> std::string str(grid_3D<T> const& v, std::string const& separator=" ", std::string const& begin="", std::string const& end="");

template <typename T> grid_3D<T>& operator+=(grid_3D<T>& a, grid_3D<T> const& b);
template <typename T> grid_3D<T>& operator+=(grid_3D<T>& a, T const& b);
template <typename T> grid_3D<T>  operator+(grid_3D<T> const& a, grid_3D<T> const& b);
template <typename T> grid_3D<T>  operator+(grid_3D<T> const& a, T const& b);
template <typename T> grid_3D<T>  operator+(T const& a, grid_3D<T> const& b);

template <typename T> grid_3D<T>& operator-=(grid_3D<T>& a, grid_3D<T> const& b);
template <typename T> grid_3D<T>& operator-=(grid_3D<T>& a, T const& b);
template <typename T> grid_3D<T>  operator-(grid_3D<T> const& a, grid_3D<T> const& b);
template <typename T> grid_3D<T>  operator-(grid_3D<T> const& a, T const& b);
template <typename T> grid_3D<T>  operator-(T const& a, grid_3D<T> const& b);

template <typename T> grid_3D<T>& operator*=(grid_3D<T>& a, grid_3D<T> const& b);
template <typename T> grid_3D<T>& operator*=(grid_3D<T>& a, float b);
template <typename T> grid_3D<T>  operator*(grid_3D<T> const& a, grid_3D<T> const& b);
template <typename T> grid_3D<T>  operator*(grid_3D<T> const& a, float b);
template <typename T> grid_3D<T>  operator*(float a, grid_3D<T> const& b);

template <typename T> grid_3D<T>& operator/=(grid_3D<T>& a, grid_3D<T> const& b);
template <typename T> grid_3D<T>& operator/=(grid_3D<T>& a, float b);
template <typename T> grid_3D<T>  operator/(grid_3D<T> const& a, grid_3D<T> const& b);
template <typename T> grid_3D<T>  operator/(grid_3D<T> const& a, float b);
template <typename T> grid_3D<T>  operator/(float a, grid_3D<T> const& b);

}



/* ************************************************** */
/*           IMPLEMENTATION                           */
/* ************************************************** */

namespace cgp
{


template <typename T>
grid_3D<T>::grid_3D()
    :dimension(int3{0,0,0}),data()
{}

template <typename T>
grid_3D<T>::grid_3D(int size)
    :dimension({size,size,size}),data(size*size*size)
{
    assert_cgp_no_msg(size>=0);
}

template <typename T>
grid_3D<T>::grid_3D(int3 const& size)
    :dimension(size),data(size[0]*size[1]*size[2])
{
    assert_cgp_no_msg(size[0]>=0 && size[1]>=0 && size[2]>=0);
}

template <typename T>
grid_3D<T>::grid_3D(int size_1, int size_2, int size_3)
    :dimension({size_1,size_2, size_3}),data(size_1*size_2*size_3)
{
    assert_cgp_no_msg(size_1>=0 && size_2>=0 && size_3>=0);
}

template <typename T>
int grid_3D<T>::size() const
{
    return dimension[0]*dimension[1]*dimension[2];
}

template <typename T>
void grid_3D<T>::resize(int size)
{
    assert_cgp_no_msg(size>=0);
    resize(size,size,size);
}

template <typename T>
void grid_3D<T>::resize(int3 const& size)
{
    assert_cgp_no_msg(size[0]>=0 && size[1]>=0 && size[2]>=0);
    dimension = size;
    data.resize(size[0]*size[1]*size[2]);
}

template <typename T>
void grid_3D<T>::resize(int size_1, int size_2, int size_3)
{
    assert_cgp_no_msg(size_1>=0 && size_2>=0 && size_3>=0);
    dimension = {size_1, size_2, size_3};
    resize({size_1, size_2, size_3});
}

template <typename T>
void grid_3D<T>::fill(T const& value)
{
    data.fill(value);
}


template <typename T>
grid_3D<T> grid_3D<T>::from_array(numarray<T> const& arg, int size_1, int size_2, int size_3)
{
    assert_cgp(arg.size()==size_1*size_2*size_3, "Incoherent size to generate grid_2D");

    grid_3D<T> b(size_1, size_2, size_3);
    b.data = arg;

    return b;
}

template <typename T>
void grid_3D<T>::clear()
{
    data.clear();
}


template <typename T>
static void check_index_bounds(int index1, int index2, int index3, grid_3D<T> const& data)
{
#ifndef cgp_NO_DEBUG
    int const N1 = data.dimension.x;
    int const N2 = data.dimension.y;
    int const N3 = data.dimension.z;
    assert_cgp_no_msg(N1>=0 && N2>=0 && N3>=0);
    if (index1 < 0 || index2 < 0 || index3 < 0 || index1 >= N1 || index2 >= N2 || index3 >= N3)
    {
        std::string msg = "\n";

        msg += "\t> Try to access grid_3D(" + str(index1) + "," + str(index2) + "," + str(index3) + ")\n";
        msg += "\t>    - grid_3D has dimension = (" + str(N1) + "," + str(N2) + "," + str(N3) + ")\n";
        msg += "\t>    - Type of grid_3D: " + type_str(data) + "\n";

        if (index1 < 0 || index2 < 0 || index3 < 0)
        {
            msg += "\t> Buffer cannot be access with negative index.\n";
        }
        else if (N1 == 0 || N2 == 0 || N3 == 0)
        {
            msg += "\t> The buffer is empty, its elements cannot be accessed.\n";
        }
        else if (index1 == N1 || index2 == N2 || index3 == N3)
        {
            msg += "\t> Index reached the maximal size of the buffer \n";
            msg += "\t> The maximal possible indexes should be (" + str(N1 - 1) + "," + str(N2 - 1) + "," + str(N3 - 1) + ") \n";
        }
        else if (index1 >= N1 || index2 >= N2 || index3 >= N3)
        {
            msg += "\t> Exceeded buffer dimension \n";
        }


        msg += "\n\t  The function and variable that generated this error can be found in analysis the Call Stack.\n";

        error_cgp(msg);
    }
#endif
}


template <typename T> T const& grid_3D<T>::operator[](int3 const& index) const
{
    check_index_bounds(index.x, index.y, index.z, *this);
    int const  idx = offset_grid(index.x, index.y, index.z, dimension.x, dimension.y);
    return data[idx];
}
template <typename T> T& grid_3D<T>::operator[](int3 const& index)
{
    check_index_bounds(index.x, index.y, index.z, *this);
    int const  idx = offset_grid(index.x, index.y, index.z, dimension.x, dimension.y);
    return data[idx];
}
template <typename T> T const& grid_3D<T>::operator()(int3 const& index) const
{
    check_index_bounds(index.x, index.y, index.z, *this);
    int const  idx = offset_grid(index.x, index.y, index.z, dimension.x, dimension.y);
    return data[idx];
}
template <typename T> T& grid_3D<T>::operator()(int3 const& index)
{
    check_index_bounds(index.x, index.y, index.z, *this);
    int const  idx = offset_grid(index.x, index.y, index.z, dimension.x, dimension.y);
    return data[idx];
}
template <typename T> T const& grid_3D<T>::operator()(int k1, int k2, int k3) const
{
    check_index_bounds(k1, k2, k3, *this);
    int const  idx = offset_grid(k1, k2, k3, dimension.x, dimension.y);
    return data[idx];
}
template <typename T> T& grid_3D<T>::operator()(int k1, int k2, int k3)
{
    check_index_bounds(k1, k2, k3, *this);
    int const  idx = offset_grid(k1, k2, k3, dimension.x, dimension.y);
    return data[idx];
}



template <typename T>
typename std::vector<T>::iterator grid_3D<T>::begin()
{
    return data.begin();
}

template <typename T>
typename std::vector<T>::iterator grid_3D<T>::end()
{
    return data.end();
}

template <typename T>
typename std::vector<T>::const_iterator grid_3D<T>::begin() const
{
    return data.begin();
}

template <typename T>
typename std::vector<T>::const_iterator grid_3D<T>::end() const
{
    return data.end();
}

template <typename T>
typename std::vector<T>::const_iterator grid_3D<T>::cbegin() const
{
    return data.cbegin();
}

template <typename T>
typename std::vector<T>::const_iterator grid_3D<T>::cend() const
{
    return data.cend();
}

template <typename T>
int grid_3D<T>::index_to_offset(int k1, int k2, int k3) const
{
    return offset_grid(k1, k2, k3, dimension.x, dimension.y);
}
template <typename T>
int grid_3D<T>::index_to_offset(int3 const& index) const
{
    return offset_grid(index, dimension.x, dimension.y);
}
template <typename T>
int3 grid_3D<T>::offset_to_index(int offset) const
{
    return index_grid_from_offset(offset, dimension.x, dimension.y);
}








template <typename T> std::string type_str(grid_3D<T> const&)
{
    return "grid_3D<" + type_str(T()) + ">";
}

template <typename T1, typename T2> bool is_equal(grid_3D<T1> const& a, grid_3D<T2> const& b)
{
    if (is_equal(a.dimension, b.dimension) == false)
        return false;
    return is_equal(a.data, b.data);
}


template <typename T> std::ostream& operator<<(std::ostream& s, grid_3D<T> const& v)
{
    return s << v.data;
}
template <typename T> std::string str(grid_3D<T> const& v, std::string const& separator, std::string const& begin, std::string const& end)
{
    return str(v.data, separator, begin, end);
}


template <typename T> grid_3D<T>& operator+=(grid_3D<T>& a, grid_3D<T> const& b)
{
    assert_cgp( is_equal(a.dimension,b.dimension), "Dimension do not agree: a:"+str(a.dimension)+", b:"+str(b.dimension) );
    a.data += b.data;
}
template <typename T> grid_3D<T>& operator+=(grid_3D<T>& a, T const& b)
{
    a.data += b;
    return a;
}
template <typename T> grid_3D<T>  operator+(grid_3D<T> const& a, grid_3D<T> const& b)
{
    assert_cgp( is_equal(a.dimension,b.dimension), "Dimension do not agree: a:"+str(a.dimension)+", b:"+str(b.dimension) );
    grid_3D<T> res(a.dimension);
    res.data = a.data+b.data;
    return res;

}
template <typename T> grid_3D<T>  operator+(grid_3D<T> const& a, T const& b)
{
    grid_3D<T> res(a.dimension);
    res.data = a.data+b;
    return res;
}
template <typename T> grid_3D<T>  operator+(T const& a, grid_3D<T> const& b)
{
    grid_3D<T> res(b.dimension);
    res.data = a + b.data;
    return res;
}

template <typename T> grid_3D<T>& operator-=(grid_3D<T>& a, grid_3D<T> const& b)
{
    assert_cgp( is_equal(a.dimension,b.dimension), "Dimension do not agree: a:"+str(a.dimension)+", b:"+str(b.dimension) );
    a.data -= b.data;
    return a;
}
template <typename T> grid_3D<T>& operator-=(grid_3D<T>& a, T const& b)
{
    a.data -= b;
    return a;
}
template <typename T> grid_3D<T>  operator-(grid_3D<T> const& a, grid_3D<T> const& b)
{
    assert_cgp( is_equal(a.dimension,b.dimension), "Dimension do not agree: a:"+str(a.dimension)+", b:"+str(b.dimension) );
    grid_3D<T> res(a.dimension);
    res.data = a.data-b.data;
    return res;
}
template <typename T> grid_3D<T>  operator-(grid_3D<T> const& a, T const& b)
{
    grid_3D<T> res(a.dimension);
    res.data = a.data-b;
    return res;
}
template <typename T> grid_3D<T>  operator-(T const& a, grid_3D<T> const& b)
{
    grid_3D<T> res(a.dimension);
    res.data = a-b.data;
    return res;
}

template <typename T> grid_3D<T>& operator*=(grid_3D<T>& a, grid_3D<T> const& b)
{
    assert_cgp( is_equal(a.dimension,b.dimension), "Dimension do not agree: a:"+str(a.dimension)+", b:"+str(b.dimension) );
    a.data *= b.data;
    return a;
}
template <typename T> grid_3D<T>& operator*=(grid_3D<T>& a, float b)
{
    a.data *= b;
    return a;
}
template <typename T> grid_3D<T>  operator*(grid_3D<T> const& a, grid_3D<T> const& b)
{
    assert_cgp( is_equal(a.dimension,b.dimension), "Dimension do not agree: a:"+str(a.dimension)+", b:"+str(b.dimension) );
    grid_3D<T> res(a.dimension);
    res.data = a.data*b.data;
    return res;
}
template <typename T> grid_3D<T>  operator*(grid_3D<T> const& a, float b)
{
    grid_3D<T> res(a.dimension);
    res.data = a.data*b;
    return res;
}
template <typename T> grid_3D<T>  operator*(float a, grid_3D<T> const& b)
{
    grid_3D<T> res(b.dimension);
    res.data = a*b.data;
    return res;
}

template <typename T> grid_3D<T>& operator/=(grid_3D<T>& a, grid_3D<T> const& b)
{
    assert_cgp( is_equal(a.dimension,b.dimension), "Dimension do not agree: a:"+str(a.dimension)+", b:"+str(b.dimension) );
    a.data /= b.data;
    return a;
}
template <typename T> grid_3D<T>& operator/=(grid_3D<T>& a, float b)
{
    a.data /= b;
    return a;
}
template <typename T> grid_3D<T>  operator/(grid_3D<T> const& a, grid_3D<T> const& b)
{
    assert_cgp( is_equal(a.dimension,b.dimension), "Dimension do not agree: a:"+str(a.dimension)+", b:"+str(b.dimension) );
    grid_3D<T> res(a.dimension);
    res.data = a.data/b.data;
    return res;
}
template <typename T> grid_3D<T>  operator/(grid_3D<T> const& a, float b)
{
    grid_3D<T> res(a.dimension);
    res.data = a.data/b;
    return res;
}
template <typename T> grid_3D<T>  operator/(float a, grid_3D<T> const& b)
{
    grid_3D<T> res(b.dimension);
    res.data = a/b.data;
    return res;
}





template <typename T>
T const& grid_3D<T>::at_unsafe(int index) const
{
    return data.at_unsafe(index);
}


template <typename T>
T & grid_3D<T>::at_unsafe(int index)
{
    return data.at_unsafe(index);
}

template <typename T>
T const& grid_3D<T>::at_unsafe(int index1, int index2, int index3) const
{
    return data.at_unsafe(offset_grid(index1, index2, index3, dimension.x, dimension.y));
}

template <typename T>
T & grid_3D<T>::at_unsafe(int index1, int index2, int index3)
{
    return data.at_unsafe(offset_grid(index1, index2, index3, dimension.x, dimension.y));
}

}
