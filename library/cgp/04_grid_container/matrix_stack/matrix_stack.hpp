#pragma once

#include "cgp/01_base/base.hpp"
#include "cgp/02_numarray/numarray_stack/numarray_stack.hpp"
#include "cgp/04_grid_container/offset_grid/offset_grid.hpp"


#include <sstream>
#include <iomanip>


/* ************************************************** */
/*           Header                                   */
/* ************************************************** */

namespace cgp
{

    /** Container for 2D-matrix structure storing elements on stack (fixed size known at compile time)
     *
     * Elements of matrix_stack are stored contiguously in stack memory as array of array and remain fully compatible with std::array and pointers.
     * Elements are stored along rows 
     * matrix[0]     (  0:[0,0]    1:[0,1]   2:[0,2] ...   N2-1:[0,N2-1] ) 
     * matrix[1]     ( N2:[1,0] N2+1:[1,1]           ... 2*N2-1:[1,N2-1] )
     *                             ...
     * matrix[k1]              k1*N2+k2:[k1,k2] 
     *                             ...    
     * matrix[N1-1]  ( (N1-1)*N2:[N1-1,0]      ...   N1*N2-1:[N1-1,N2-1] )
     **/
    template <typename T, int N1, int N2>
    struct matrix_stack
    {
        /** Internal storage as a 1D buffer */
        numarray_stack< numarray_stack<T, N2>, N1> data;

        /** Constructors */
        matrix_stack();
        matrix_stack(numarray_stack< numarray_stack<T, N2>, N1> const& elements);
        matrix_stack(numarray_stack<T, N1* N2> const& elements);

        // Construct from a matrix with different size.
        //  Consider the min between (N1,N1_arg) and (N2,N2_arg)
        template <int N1_arg, int N2_arg>
        explicit matrix_stack(matrix_stack<T, N1_arg, N2_arg> const& M);

        matrix_stack(std::initializer_list<T> const& arg);
        matrix_stack(std::initializer_list<numarray_stack<T,N1> > const& arg);

        static matrix_stack<T, N1, N2> build_identity();
        static matrix_stack<T, N1, N2> diagonal(numarray_stack<T, std::min(N1,N2)> const& arg);

        /** Total number of elements size = dimension[0] * dimension[1] */
        int size() const;
        /** Return {N1,N2} */
        int2 dimension() const;
        /** Fill all elements of the grid_2D with the same element*/
        matrix_stack<T, N1, N2>& fill(T const& value);


        /** Element access
         * Bound checking is performed unless cgp_NO_DEBUG is defined. */
        numarray_stack<T, N2> const& operator[](int k1) const;
        numarray_stack<T, N2>& operator[](int k1);

        numarray_stack<T, N2> const& operator()(int k1) const;
        numarray_stack<T, N2>& operator()(int k1);

        T const& operator()(int k1, int k2) const;
        T& operator()(int k1, int k2);

        T const& at_offset(int offset) const;
        T& at_offset(int offset);


        matrix_stack<T, N1 - 1, N2 - 1> remove_row_column(int k1, int k2) const;
        
        /** Set a block within the matrix from a specific offset 
        *    @block: the matrix to be copied in the current one
        *    @offset: the offsets where the block has to be writter. Default value are (0,0). The block is written on the top-left corner.
        * Conditions:
        *     offset_1 + N1_arg < N1
        *     offset_2 + N2_arg < N2
        */
        template <int N1_arg, int N2_arg>
        matrix_stack<T, N1, N2>& set_block(matrix_stack<T, N1_arg, N2_arg> const& block, int offset_1 = 0, int offset_2 = 0);



        /** Iterators
        *  Iterators compatible with STL syntax and std::array */
        T* begin();
        T* end();
        T const* begin() const;
        T const* end() const;
        T const* cbegin() const;
        T const* cend() const;


        inline T const& at(int k1, int k2) const;
        inline T& at(int k1, int k2);

        T const& at_unsafe(int k1, int k2) const;
        T& at_unsafe(int k1, int k2);

        numarray_stack<T, N2> const& at_unsafe(int k1) const;
        numarray_stack<T, N2>& at_unsafe(int k1);

        T const& at_offset_unsafe(int offset) const;
        T& at_offset_unsafe(int offset);
    };


    template <typename T, int N1, int N2> std::string type_str(matrix_stack<T, N1, N2> const&);

    /** Display all elements of the buffer.*/
    template <typename T, int N1, int N2> std::ostream& operator<<(std::ostream& s, matrix_stack<T, N1, N2> const& v);

    /** Convert all elements of the matrix to a string.
    *  Default behavior: display all elements separated by a space as a 1D buffer
    * 
     * [begin]
     *   [begin_line] v(0,0)    [separator] v(0,1)    [separator] ... v(0,N2-1)    [end_line]
     *   [begin_line] v(1,0)    [separator] v(1,1)    [separator] ... v(1,N2-1)    [end_line]
     *    ...
     *   [begin_line] v(N1-1,0) [separator] v(N1-1,1) [separator] ... v(N1-1,N2-1) [end_line]
     *   ...
     * [end]
     */
    template <typename T, int N1, int N2> std::string str(matrix_stack<T, N1, N2> const& v, std::string const& separator = " ", std::string const& begin = "", std::string const& end = "", std::string const& begin_line="", std::string const& end_line=" ");

    /** Convert element of the matrix to a string. Default set for pretty 2D display. */
    template <typename T, int N1, int N2> std::string str_pretty(matrix_stack<T, N1, N2> const& M, std::string const& separator=" ", std::string const& begin="", std::string const& end="", std::string const& begin_line="(", std::string const& end_line=")\n");

    template <typename T, int N1, int N2> T const* ptr(matrix_stack<T,N1,N2> const& M);
    template <typename T, int N1, int N2> int size_in_memory(matrix_stack<T,N1,N2> const& M);



    /** Direct compiled-checked access to data */
    template <int idx1, int idx2, typename T, int N1, int N2> T const& get(matrix_stack<T, N1, N2> const& data);
    template <int idx1, int idx2, typename T, int N1, int N2> T& get(matrix_stack<T, N1, N2>& data);
    template <int idx1, typename T, int N1, int N2> numarray_stack<T, N2> const& get(matrix_stack<T, N1, N2> const& data);
    template <int idx1, typename T, int N1, int N2> numarray_stack<T, N2>& get(matrix_stack<T, N1, N2>& data);
    template <int offset, typename T, int N1, int N2> T const& get_offset(matrix_stack<T, N1, N2> const& data);
    template <int offset, typename T, int N1, int N2> T& get_offset(matrix_stack<T, N1, N2>& data);

    /** Equality test between grid_2D */
    template <typename Ta, typename Tb, int Na1, int Na2, int Nb1, int Nb2> bool is_equal(matrix_stack<Ta, Na1, Na2> const& a, matrix_stack<Tb, Nb1, Nb2> const& b);

    /** Math operators
     * Common mathematical operations between buffers, and scalar or element values. */
    template <typename T, int N1, int N2> matrix_stack<T, N1, N2>& operator+=(matrix_stack<T, N1, N2>& a, matrix_stack<T, N1, N2> const& b);
    template <typename T, int N1, int N2> matrix_stack<T, N1, N2>  operator+(matrix_stack<T, N1, N2> const& a, matrix_stack<T, N1, N2> const& b);

    template <typename T, int N1, int N2> matrix_stack<T, N1, N2>& operator-=(matrix_stack<T, N1, N2>& a, matrix_stack<T, N1, N2> const& b);
    template <typename T, int N1, int N2> matrix_stack<T, N1, N2>  operator-(matrix_stack<T, N1, N2> const& a, matrix_stack<T, N1, N2> const& b);


    template <typename T, int N> matrix_stack<T, N, N>& operator*=(matrix_stack<T, N, N>& a, matrix_stack<T, N, N> const& b);
    template <typename T, int N1, int N2> matrix_stack<T, N1, N2>& operator*=(matrix_stack<T, N1, N2>& a, float b);
    template <typename T, int N1, int N2, int N3> matrix_stack<T, N1, N3>  operator*(matrix_stack<T, N1, N2> const& a, matrix_stack<T, N2, N3> const& b);
    template <typename T, int N1, int N2> matrix_stack<T, N1, N2>  operator*(matrix_stack<T, N1, N2> const& a, float b);
    template <typename T, int N1, int N2> matrix_stack<T, N1, N2>  operator*(float a, matrix_stack<T, N1, N2> const& b);

    template <typename T, int N1, int N2> matrix_stack<T, N1, N2>& operator/=(matrix_stack<T, N1, N2>& a, float b);
    template <typename T, int N1, int N2> matrix_stack<T, N1, N2>  operator/(matrix_stack<T, N1, N2> const& a, float b);

    // Unary negation
    template <typename T, int N1, int N2> matrix_stack<T, N1, N2> operator-(matrix_stack<T, N1, N2> const& m);

    // Componentwise multiplication between two matrices with the same size
    template <typename T, int N1, int N2> matrix_stack<T, N1, N2> multiply_componentwise(matrix_stack<T, N1, N2> const& a, matrix_stack<T, N1, N2> const& b);

    // Matrix vector product
    template <typename T, int N1, int N2> numarray_stack<T, N1> operator*(matrix_stack<T, N1, N2> const& a, numarray_stack<T, N2> const& b);

    /** Transposition of matrix */
    template <typename T, int N1, int N2> matrix_stack<T, N2, N1> transpose(matrix_stack<T, N1, N2> const& m);

    /** Trace of a square matrix*/
    template <typename T, int N> T trace(matrix_stack<T, N, N> const& m);

    /** Componentwise norm : sqrt(sum_(i,j) a_ij^2) */
    template <typename T, int N1, int N2> T norm(matrix_stack<T,N1,N2> const& m);

    template <typename T, int N1, int N2>
	std::istream& read_from_stream(std::istream& stream, matrix_stack<T, N1, N2>& data);
}



/* ************************************************** */
/*           IMPLEMENTATION                           */
/* ************************************************** */


namespace cgp
{

    template <typename T, int N1, int N2>
    T const& matrix_stack<T, N1, N2>::at(int k1, int k2) const
    {
        return *(begin() + k2 + N2 * k1);
    }

    template <typename T, int N1, int N2>
    T& matrix_stack<T, N1, N2>::at(int k1, int k2)
    {
        return *(begin() + k2 + N2 * k1);
    }


    template <typename T, int N1, int N2> T* matrix_stack<T, N1, N2>::begin() { return &at_unsafe(0, 0); }
    template <typename T, int N1, int N2> T* matrix_stack<T, N1, N2>::end() { return &at_unsafe(N1-1, N2-1)+1; }
    template <typename T, int N1, int N2> T const* matrix_stack<T, N1, N2>::begin() const { return &at_unsafe(0, 0); }
    template <typename T, int N1, int N2> T const* matrix_stack<T, N1, N2>::end() const { return &at_unsafe(N1 - 1, N2 - 1) + 1; }
    template <typename T, int N1, int N2> T const* matrix_stack<T, N1, N2>::cbegin() const { return &at_unsafe(0, 0); }
    template <typename T, int N1, int N2> T const* matrix_stack<T, N1, N2>::cend() const { return &at_unsafe(N1 - 1, N2 - 1) + 1; }

    template <typename T, int N1, int N2> T const& matrix_stack<T, N1, N2>::at_unsafe(int k1, int k2) const { return data.at_unsafe(k1).at_unsafe(k2); }
    template <typename T, int N1, int N2> T& matrix_stack<T, N1, N2>::at_unsafe(int k1, int k2) { return data.at_unsafe(k1).at_unsafe(k2); }


    template <typename T, int N1, int N2> numarray_stack<T, N2> const& matrix_stack<T, N1, N2>::at_unsafe(int k1) const { return data.at_unsafe(k1); }
    template <typename T, int N1, int N2> numarray_stack<T, N2>& matrix_stack<T, N1, N2>::at_unsafe(int k1) { return data.at_unsafe(k1); }

    template <typename T, int N1, int N2> T const& matrix_stack<T, N1, N2>::at_offset_unsafe(int offset) const { return begin()[offset];}
    template <typename T, int N1, int N2> T& matrix_stack<T, N1, N2>::at_offset_unsafe(int offset) { return begin()[offset]; }


    template <typename T, int N1, int N2>
    matrix_stack<T, N1, N2>::matrix_stack()
        : data()
    {}

    template <typename T, int N1, int N2>
    matrix_stack<T, N1, N2>::matrix_stack(numarray_stack< numarray_stack<T, N2>, N1> const& elements)
        :data(elements)
    {}


    template <typename T, int N1, int N2>
    matrix_stack<T, N1, N2>::matrix_stack(numarray_stack<T, N1* N2> const& elements)
        : data()
    {
        int counter = 0;
        for (int k1 = 0; k1 < N1; ++k1)
            for (int k2 = 0; k2 < N2; ++k2) {
                at_unsafe(k1, k2) = elements.at_unsafe(counter);
                counter++;
            }
    }

    template <typename T, int N1, int N2>
    matrix_stack<T, N1, N2>::matrix_stack(std::initializer_list<T> const& arg)
        : data()
    {
        assert_cgp(arg.size() >= N1 * N2, "Insufficient size to initialize matrix_stack");
        auto it_arg = arg.begin();

        for (int k1 = 0; k1 < N1; ++k1) {
            for (int k2 = 0; k2 < N2; ++k2) {
                at_unsafe(k1, k2) = *it_arg;
                ++it_arg;
            }
        }
    }

    template <typename T, int N1, int N2>
    matrix_stack<T, N1, N2>::matrix_stack(std::initializer_list<numarray_stack<T, N1> > const& arg)
        :data()
    {
        
        assert_cgp(arg.size() >= N1, "Insufficient size to initialize matrix_stack");
        auto it_arg = arg.begin();
        for (int k1 = 0; k1 < N1; ++k1) {
            data.at_unsafe(k1) = *it_arg;
            ++it_arg;
        }
    }

    template <typename T, int N1, int N2>
    template <int N1_arg, int N2_arg>
    matrix_stack<T, N1, N2>::matrix_stack(matrix_stack<T, N1_arg, N2_arg> const& M)
        :data()
    {
        int const N1m = std::min(N1, N1_arg);
        int const N2m = std::min(N2, N2_arg);
        for (int k1 = 0; k1 < N1m; ++k1)
            for (int k2 = 0; k2 < N2m; ++k2)
                at_unsafe(k1, k2) = M.at_unsafe(k1, k2);
    }


    template <typename T, int N1, int N2> int matrix_stack<T, N1, N2>::size() const { return N1 * N2; }
    template <typename T, int N1, int N2> int2 matrix_stack<T, N1, N2>::dimension() const { return { N1,N2 }; }
    template <typename T, int N1, int N2> matrix_stack<T, N1, N2>& matrix_stack<T, N1, N2>::fill(T const& value)
    {
        auto it = begin();
        auto const it_end = end();
        for (; it != it_end; ++it)
            *it = value;
        return *this;
    }



    template <typename T, int N1, int N2>
    void check_index_bounds(int index1, int index2, matrix_stack<T, N1, N2> const& data)
    {
#ifndef cgp_NO_DEBUG
        if (index1 < 0 || index2 < 0 || index1 >= N1 || index2 >= N2)
        {
            std::string msg = "\n";

            msg += "\t> Try to access matrix_stack(" + str(index1) + "," + str(index2) + ")\n";
            msg += "\t>    - matrix_stack has dimension = (" + str(N1) + "," + str(N2) + ")\n";
            msg += "\t>    - Type of matrix_stack: " + type_str(data) + "\n";

            if (index1 < 0 || index2 < 0)
            {
                msg += "\t> Buffer cannot be access with negative index.\n";
            }
            else if (N1 == 0 || N2 == 0)
            {
                msg += "\t> The buffer is empty, its elements cannot be accessed.\n";
            }
            else if (index1 == N1 || index2 == N2)
            {
                msg += "\t> Index reached the maximal size of the buffer \n";
                msg += "\t> The maximal possible indexes should be (" + str(N1 - 1) + "," + str(N2 - 1) + ") \n";
            }
            else if (index1 >= N1 || index2 >= N2)
            {
                msg += "\t> Exceeded buffer dimension \n";
            }


            msg += "\n\t  The function and variable that generated this error can be found in analysis the Call Stack.\n";

            error_cgp(msg);
        }
#endif
    }

    template <typename T, int N1, int N2>
    void check_index_bounds(int index2, matrix_stack<T, N1, N2> const& data)
    {
#ifndef cgp_NO_DEBUG
        if (index2 < 0 || index2 >= N2)
        {
            std::string msg = "\n";

            msg += "\t> Try to access matrix_stack(" + str(index2) + ")\n";
            msg += "\t>    - matrix_stack has dimension = (" + str(N1) + "," + str(N2) + ")\n";
            msg += "\t>    - maximal first index is = (" + str(N2-1) + ")\n";
            msg += "\t>    - Type of matrix_stack: " + type_str(data) + "\n";

            if (index2 < 0)
            {
                msg += "\t> Buffer cannot be access with negative index.\n";
            }
            else if (N2 == 0)
            {
                msg += "\t> The buffer is empty, its elements cannot be accessed.\n";
            }
            else if (index2 == N2)
            {
                msg += "\t> Index reached the maximal size of the buffer \n";
                msg += "\t> The maximal possible indexes should be (" + str(N1 - 1) + "," + str(N2 - 1) + ") \n";
            }
            else if (index2 >= N2)
            {
                msg += "\t> Exceeded buffer dimension \n";
            }


            msg += "\n\t  The function and variable that generated this error can be found in analysis the Call Stack.\n";

            error_cgp(msg);
        }
#endif
    }

    template <typename T, int N1, int N2>
    void check_offset_bounds(int offset, matrix_stack<T, N1, N2> const& data)
    {
#ifndef cgp_NO_DEBUG
        if (offset < 0 || offset >= N1*N2 )
        {
            std::string msg = "\n";

            msg += "\t> Try to access matrix_stack.at_offset(" + str(offset) + ")\n";
            msg += "\t>    - matrix_stack has dimension = (" + str(N1) + "," + str(N2) + ")\n";
            msg += "\t>    - the maximal offset is = (" + str(N1*N2) + ")\n";
            msg += "\t>    - Type of matrix_stack: " + type_str(data) + "\n";

            if (offset < 0)
            {
                msg += "\t> Buffer cannot be access with negative index.\n";
            }
            else if (offset == 0)
            {
                msg += "\t> The buffer is empty, its elements cannot be accessed.\n";
            }
            else if (offset == N1*N2)
            {
                msg += "\t> Offset reached the maximal size of the buffer \n";
                msg += "\t> The maximal possible offset should be (" + str( N1*N2 - 1) + ") \n";
            }
            else if (offset >= N1*N2 )
            {
                msg += "\t> Exceeded buffer dimension \n";
            }


            msg += "\n\t  The function and variable that generated this error can be found in analysis the Call Stack.\n";

            error_cgp(msg);
        }
#endif
    }


    template <typename T, int N1, int N2>
    numarray_stack<T, N2> const& matrix_stack<T, N1, N2>::operator[](int k1) const {
        check_index_bounds(k1, *this);
        return at_unsafe(k1);
    }
    template <typename T, int N1, int N2>
    numarray_stack<T, N2>& matrix_stack<T, N1, N2>::operator[](int k1) {
        check_index_bounds(k1, *this);
        return at_unsafe(k1);
    }

    template <typename T, int N1, int N2>
    numarray_stack<T, N2> const& matrix_stack<T, N1, N2>::operator()(int k1) const {
        check_index_bounds(k1, *this);
        return at_unsafe(k1);
    }
    template <typename T, int N1, int N2>
    numarray_stack<T, N2>& matrix_stack<T, N1, N2>::operator()(int k1) {
        check_index_bounds(k1, *this);
        return at_unsafe(k1);
    }


    template <typename T, int N1, int N2>
    T const& matrix_stack<T, N1, N2>::operator()(int k1, int k2) const {
        check_index_bounds(k1, k2, *this);
        return at_unsafe(k1,k2);
    }
    template <typename T, int N1, int N2>
    T& matrix_stack<T, N1, N2>::operator()(int k1, int k2) {
        check_index_bounds(k1, k2, *this);
        return at_unsafe(k1, k2);
    }


    template <typename T, int N1, int N2>
    T const& matrix_stack<T, N1, N2>::at_offset(int offset) const {
        check_offset_bounds(offset, *this);
        return at_offset_unsafe(offset);
    }
    template <typename T, int N1, int N2>
    T& matrix_stack<T, N1, N2>::at_offset(int offset) {
        check_offset_bounds(offset, *this);
        return at_offset_unsafe(offset);
    }


    template <typename T, int N1, int N2>
    template <int N1_arg, int N2_arg>
    matrix_stack<T, N1, N2>& matrix_stack<T, N1, N2>::set_block(matrix_stack<T, N1_arg, N2_arg> const& block, int offset_1, int offset_2)
    {
        static_assert(N1_arg < N1, "Block size is too large for the current matrix");
        static_assert(N2_arg < N2, "Block size is too large for the current matrix");
        assert_cgp(N1_arg + offset_1 < N1, "Block size exceed current matrix size");
        assert_cgp(N2_arg + offset_2 < N2, "Block size exceed current matrix size");

        for (int k1 = 0; k1 < N1_arg; ++k1) {
            int const idx_1 = k1 + offset_1;
            for (int k2 = 0; k2 < N2_arg; ++k2) {
                int const idx_2 = k2 + offset_2;
                at_unsafe(idx_1, idx_2) = block.at_unsafe(k1, k2);
            }
        }
        return *this;
    }


}





namespace cgp
{



    template <typename T, int N1, int N2> std::string type_str(matrix_stack<T, N1, N2> const&)
    {
        return "matrix_stack<" + type_str(T()) + "," + str(N1) + "," + str(N2) + ">";
    }


    template <typename Ta, typename Tb, int Na1, int Na2, int Nb1, int Nb2> bool is_equal(matrix_stack<Ta, Na1, Na2> const& a, matrix_stack<Tb, Nb1, Nb2> const& b)
    {
        if (Na1 != Nb1 || Na2 != Nb2)
            return false;
        return is_equal(a.data, b.data);
    }

    template <typename T, int N1, int N2> T const* ptr(matrix_stack<T, N1, N2> const& M)
    {
        return &get<0,0>(M);
    }
    template <typename T, int N1, int N2> int size_in_memory(matrix_stack<T, N1, N2> const& )
    {
        return size_in_memory(T{})*N1*N2;
    }

    template <int idx1, int idx2, typename T, int N1, int N2> T const& get(matrix_stack<T, N1, N2> const& data)
    {
        static_assert( (idx1 < N1) && (idx2 < N2), "Index too large for matrix_stack access");
        return data.at(idx1, idx2);
    }
    template <int idx1, int idx2, typename T, int N1, int N2> T& get(matrix_stack<T, N1, N2>& data)
    {
        static_assert((idx1 < N1) && (idx2 < N2), "Index too large for matrix_stack access");
        return data.at(idx1, idx2);
    }
    template <int idx1, typename T, int N1, int N2> numarray_stack<T, N2> const& get(matrix_stack<T, N1, N2> const& data)
    {
        static_assert(idx1<N1, "Index too large for matrix_stack access");
        return get<idx1>(data.data);
    }
    template <int idx1, typename T, int N1, int N2> numarray_stack<T, N2>& get(matrix_stack<T, N1, N2>& data)
    {
        static_assert(idx1 < N1, "Index too large for matrix_stack access");
        return get<idx1>(data.data);
    }

    template <int offset, typename T, int N1, int N2> T const& get_offset(matrix_stack<T, N1, N2> const& data)
    {
        static_assert(offset<N1*N2, "Index too large for matrix_stack access");
        return data.at_offset_unsafe(offset);
    }
    template <int offset, typename T, int N1, int N2> T& get_offset(matrix_stack<T, N1, N2>& data)
    {
        static_assert(offset < N1* N2, "Index too large for matrix_stack access");
        return data.at_offset_unsafe(offset);
    }


    template <typename T, int N1, int N2> std::ostream& operator<<(std::ostream& s, matrix_stack<T, N1, N2> const& v)
    {
        return s << v.data;
    }
    template <typename T, int N1, int N2> std::string str(matrix_stack<T, N1, N2> const& v, std::string const& separator, std::string const& begin, std::string const& end, std::string const& begin_line, std::string const& end_line)
    {
        std::string s;
        s += begin;
        for (int k1 = 0; k1 < N1; ++k1) {
            s += begin_line;
            for (int k2 = 0; k2 < N2; ++k2) {
                s += str(v.at_unsafe(k1, k2));
                if ( k2 != N2 - 1)
                    s += separator;
            }
            s += end_line;
        }
        s += end;
        return s;
    }
    template <typename T,int N1, int N2> std::string str_pretty(matrix_stack<T, N1, N2> const& M, std::string const& separator, std::string const& begin, std::string const& end, std::string const& begin_line, std::string const& end_line)
	{
		std::string s;
        std::stringstream ss;
        ss << begin;
        for (int k1 = 0; k1 < N1; ++k1) {
            ss << begin_line;
            for (int k2 = 0; k2 < N2; ++k2) {
                ss <<std::fixed<<std::setprecision(2) << std::setw(7) << M.at(k1,k2);
                if ( k2 != N2 - 1 )
                    ss << separator;
            }
            ss << end_line;
        }
        ss << end;
        return ss.str();
	}

    template <typename T, int N1, int N2> std::string str_2D(matrix_stack<T, N1, N2> const& v, std::string const& separator, std::string const& begin, std::string const& end, std::string const& begin_line, std::string const& end_line)
    {
        return str(v, separator, begin, end, begin_line, end_line);
    }


    template <typename T, int N1, int N2> matrix_stack<T, N1, N2>& operator+=(matrix_stack<T, N1, N2>& a, matrix_stack<T, N1, N2> const& b)
    {
        a.data += b.data;
        return a;
    }
    template <typename T, int N1, int N2> matrix_stack<T, N1, N2>& operator+=(matrix_stack<T, N1, N2>& a, T const& b)
    {
        a.data += b;
        return a;
    }
    template <typename T, int N1, int N2> matrix_stack<T, N1, N2>  operator+(matrix_stack<T, N1, N2> const& a, matrix_stack<T, N1, N2> const& b)
    {
        matrix_stack<T, N1, N2> res;
        res.data = a.data + b.data;
        return res;

    }
    template <typename T, int N1, int N2> matrix_stack<T, N1, N2> operator+(matrix_stack<T, N1, N2> const& a, T const& b)
    {
        matrix_stack<T, N1, N2> res;
        res.data = a.data + b;
        return res;
    }
    template <typename T, int N1, int N2> matrix_stack<T, N1, N2>  operator+(T const& a, matrix_stack<T, N1, N2> const& b)
    {
        matrix_stack<T, N1, N2> res;
        res.data = a + b.data;
        return res;
    }

    template <typename T, int N1, int N2> matrix_stack<T, N1, N2>& operator-=(matrix_stack<T, N1, N2>& a, matrix_stack<T, N1, N2> const& b)
    {
        a.data -= b.data;
        return a;
    }

    template <typename T, int N1, int N2> matrix_stack<T, N1, N2>  operator-(matrix_stack<T, N1, N2> const& a, matrix_stack<T, N1, N2> const& b)
    {
        matrix_stack<T, N1, N2> res;
        res.data = a.data - b.data;
        return res;
    }


    template <typename T, int N> matrix_stack<T, N, N>& operator*=(matrix_stack<T, N, N>& a, matrix_stack<T, N, N> const& b)
    {
        matrix_stack<T, N, N> res = a * b;
        a = res;
        return a;
    }
    template <typename T, int N1, int N2> matrix_stack<T, N1, N2>& operator*=(matrix_stack<T, N1, N2>& a, float b)
    {
        a.data *= b;
        return a;
    }
    template <typename T, int N1, int N2, int N3> matrix_stack<T, N1, N3>  operator*(matrix_stack<T, N1, N2> const& a, matrix_stack<T, N2, N3> const& b)
    {
        matrix_stack<T, N1, N3> res;
        for (int k1 = 0; k1 < N1; ++k1)
        {
            for (int k3 = 0; k3 < N3; ++k3)
            {
                T s {};
                for (int k2 = 0; k2 < N2; ++k2)
                    s += a.at(k1, k2) * b.at(k2, k3);
                res(k1, k3) = s;
            }
        }

        return res;
    }

    template <typename T, int N1, int N2> matrix_stack<T, N1, N2> operator*(matrix_stack<T, N1, N2> const& a, float b)
    {
        matrix_stack<T, N1, N2> res;
        res.data = a.data * b;
        return res;
    }
    template <typename T, int N1, int N2> matrix_stack<T, N1, N2> operator*(float a, matrix_stack<T, N1, N2> const& b)
    {
        matrix_stack<T, N1, N2> res;
        res.data = a * b.data;
        return res;
    }

    template <typename T, int N1, int N2> matrix_stack<T, N1, N2>& operator/=(matrix_stack<T, N1, N2>& a, float b)
    {
        a.data /= b;
        return a;
    }
    template <typename T, int N1, int N2> matrix_stack<T, N1, N2>  operator/(matrix_stack<T, N1, N2> const& a, float b)
    {
        matrix_stack<T, N1, N2> res;
        res.data = a.data / b;
        return res;
    }

    template <typename T, int N1, int N2> matrix_stack<T, N1, N2> operator-(matrix_stack<T, N1, N2> const& m)
    {
        matrix_stack<T, N1, N2> res = m;
        res *= -1.0f;
        return res;
    }

    template <typename T, int N1, int N2> matrix_stack<T, N1, N2> multiply_componentwise(matrix_stack<T, N1, N2> const& a, matrix_stack<T, N1, N2> const& b)
    {
        matrix_stack<T, N1, N2> res;
        res.data = a.data * b.data;
        return res;
    }

    template <typename T, int N1, int N2> numarray_stack<T, N1> operator*(matrix_stack<T, N1, N2> const& a, numarray_stack<T, N2> const& b)
    {
        numarray_stack<T, N1> res = {};
        for (int k1 = 0; k1 < N1; ++k1) {
            auto& current = res.at_unsafe(k1);
            for (int k2 = 0; k2 < N2; ++k2)
                current += a.at(k1,k2) * b.at(k2);
            res.at_unsafe(k1) = current;
        }
        return res;
    }

    template <typename T, int N1, int N2> matrix_stack<T, N2, N1> transpose(matrix_stack<T, N1, N2> const& m)
    {
        matrix_stack<T, N2, N1> res;
        for (int k1 = 0; k1 < N1; ++k1)
            for (int k2 = 0; k2 < N2; ++k2)
                res(k2, k1) = m(k1, k2);
        return res;
    }


    template <typename T, int N1, int N2>
    matrix_stack<T, N1 - 1, N2 - 1> matrix_stack<T, N1, N2>::remove_row_column(int idx1, int idx2) const
    {
        assert_cgp( (idx1 < N1) && (idx2 < N2), "Incorrect index for removing row and column to matrix");
        matrix_stack<T, N1 - 1, N2 - 1> res;

        int k1_res = 0;
        for (int k1 = 0; k1 < N1; ++k1) {
            if (k1 != idx1) {
                int k2_res = 0;
                for (int k2 = 0; k2 < N2; ++k2) {
                    if (k2 != idx2){
                        res.at_unsafe(k1_res, k2_res) = at_unsafe(k1, k2);
                        ++k2_res;
                    }
                }
                ++k1_res;
            }
        }

        return res;
    }

    template <typename T, int N1, int N2>
    matrix_stack<T, N1, N2> matrix_stack<T, N1, N2>::build_identity()
    {
        int const N = std::min(N1, N2);
        matrix_stack<T, N1, N2> id = {};
        for (int k = 0; k < N; ++k)
            id.at_unsafe(k, k) = cgp_trait<T>::one();
        return id;
    }

    template <typename T, int N1, int N2>
    matrix_stack<T, N1, N2> matrix_stack<T, N1, N2>::diagonal(numarray_stack<T, std::min(N1, N2)> const& arg)
    {
        int const N = std::min(N1, N2);
        matrix_stack<T, N1, N2> m = {};
        for (int k = 0; k < N; ++k)
            m.at_unsafe(k, k) = arg[k];
        return m;
    }

    
    template <typename T, int N1, int N2> T norm(matrix_stack<T, N1, N2> const& m)
    {
        using std::sqrt; 

        T s{};
        for(int k1=0; k1<N1; ++k1)
            for(int k2=0; k2<N2; ++k2)
                s += m.at_unsafe(k1,k2);

        return sqrt(s);
    }

    template <typename T, int N> T trace(matrix_stack<T, N, N> const& m)
    {
        T s = {};
        for (int k = 0; k < N; ++k)
            s += m(k, k);
        return s;
    }

    template <typename T, int N1, int N2>
	std::istream& read_from_stream(std::istream& stream, matrix_stack<T, N1, N2>& data)
	{
        for (int k1 = 0; k1 < N1 && stream.good(); ++k1){
            for (int k2 = 0; k2 < N2 && stream.good(); ++k2){
                read_from_stream(stream, data[k1][k2]);
            }
        }
		return stream;
	}

}


#include "special_types/special_types.hpp"