#pragma once

#include <string>
#include <vector>
#include <array>

#include "../basic_types/basic_types.hpp"

// Helper functions with respect to string handling and conversion

namespace cgp
{

/** Convert a generic type to a string */
//template <typename T> std::string str(const T& x);
/** Convert a generic type to a string */
//template <typename T> std::string to_string(const T& x);

/** add zeros in from of the string */
std::string zero_fill(const std::string& input, size_t number_of_zero);


///** Allows to call to_string on itself */
std::string const& str(std::string const& s);
///** Allows to call to_string a C-string */
std::string str(char const* s);



/** Check equality between string (using ==) */
bool is_equal(std::string const& a, std::string const& b);
bool is_equal(std::string const& a, char const* b);
bool is_equal(char const* a, std::string const& b);

std::string type_str(std::string const& s);
template <typename T> std::string type_str(T const&);

namespace detail{

/** Generic template function to export string from a container */
template <typename T>
std::string str_container(T const& v,
                          std::string const& separator=" ",
                          std::string const& begin="",
                          std::string const& end="");
}


// Template implementation




//
//template <typename T> std::string str(const T& x)
//{
//    using cgp::to_string;
//    return to_string(x);
//}
//
//template <typename T> std::string to_string(const T& x)
//{
//    return std::to_string(x);
//}

//template <typename T, size_t N>
//std::string to_string(std::array<T,N> v, const std::string& separator, std::string const& begin, std::string const& end)
//{
//    return detail::to_string_container(v, separator, begin, end);
//}
//
//template <typename T>
//std::string to_string(std::vector<T> v, const std::string& separator, std::string const& begin, std::string const& end)
//{
//    return detail::to_string_container(v, separator, begin, end);
//}

template <typename T> std::string type_str(T const&)
{
    return "<Function type_str not defined for this type>";
}


namespace detail{
template <typename T> std::string str_container(T const& v,
                                                std::string const& separator,
                                                std::string const& begin,
                                                std::string const& end)
{
    if( v.size()==0 )
        return begin+end;

    std::string s;
    s += begin;

    auto it = v.cbegin();
    auto it_next = it; ++it_next;
    auto const it_end = v.cend();

    using cgp::str;
    while( it!=it_end )
    {
        s += str(*it);
        if(it_next!=it_end)
            s += separator;

        
        if (it != it_end) ++it;
        if (it_next != it_end) ++it_next;
    }
    s += end;

    return s;
}
}



}
