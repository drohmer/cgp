#pragma once

#include "../basic_types/basic_types.hpp"

#include <array>
#include <map>
#include <vector>
#include <set>


namespace cgp
{

// Check (possibly approximate) equality between 2 arrays
template <typename T1, typename T2, size_t N>
bool is_equal( std::array<T1,N> const& a, std::array<T2,N> const& b );

// Check (possibly approximate) equality between 2 vector
template <typename T1, typename T2>
bool is_equal(std::vector<T1> const& a, std::vector<T2> const& b);

template <typename T>
size_t size_in_memory(std::vector<T> const& v);

template <typename T>
auto const* ptr(std::vector<T> const& v);


}

// Template implementation

namespace cgp
{

template <typename T1, typename T2, size_t N>
bool is_equal( std::array<T1,N> const& a, std::array<T2,N> const& b )
{
    for(size_t k=0; k<N; ++k)
        if( is_equal(a[k],b[k])==false )
            return false;
    return true;
}

template <typename T1, typename T2>
bool is_equal(std::vector<T1> const& a, std::vector<T2> const& b)
{
    if (a.size() != b.size())
        return false;

    size_t const N = a.size();
    for (size_t k = 0; k < N; ++k)
        if (is_equal(a[k], b[k]) == false)
            return false;
    return true;
}

// Check if a key exists in the map
template <typename T1, typename T2> bool contains(std::map<T1, T2> const& m, T1 const& key)
{
    if( m.find(key)==m.end() )
        return false;
    return true;
}
template <typename T> bool contains(std::map<std::string, T> const& m, char const* key)
{
    if( m.find(key)==m.end() )
        return false;
    return true;
}

// helper function to print a map
template <typename T1, typename T2> std::string str(std::map<T1, T2> const& m)
{
    std::string str = "{";
    for (auto const& element : m) {
        str += cgp::str(element.first)+":"+cgp::str(element.second)+" ";
    }
    str +="}";
    return str;
}

template <typename T> std::string type_str(std::set<T> const&)
{
    using cgp::type_str;
    return "std::set<"+type_str(T())+">";
}

template <typename T>
size_t size_in_memory(std::vector<T> const& v)
{
    size_t s = 0;
    for (int k = 0; k < v.size(); ++k)
        s += size_in_memory(v[k]);
    return s;
}

template <typename T>
auto const* ptr(std::vector<T> const& v)
{
    using cgp::ptr;
    return ptr(v[0]);
}

}
