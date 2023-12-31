#pragma once

#include <cstddef>
#include <string>


namespace cgp
{

    constexpr float Pi = 3.14159f;


// Uniform equality test function is_equal()
bool is_equal(float a, float b);                // |a-b|<epsilon
bool is_equal(double a, double b);              // |a-b|<epsilon
// mixed equality
bool is_equal(float a, double b);
bool is_equal(double a, float b);

template <typename T1, typename T2>
bool is_equal(T1 const& a, T2 const& b) { return a==b; }


// Uniform size() function (number of elements) for all types - return 1 for fundamental types
constexpr size_t size(float a);
constexpr size_t size(double a);
constexpr size_t size(char a);
constexpr size_t size(short a);
constexpr size_t size(int a);
constexpr size_t size(long a);
constexpr size_t size(long long a);
constexpr size_t size(unsigned char a);
constexpr size_t size(unsigned short a);
constexpr size_t size(unsigned int a);
constexpr size_t size(unsigned long a);
constexpr size_t size(unsigned long long a);

// Size took in memory for the variable (in Byte)
size_t size_in_memory(float a);
size_t size_in_memory(double a);
size_t size_in_memory(char a);
size_t size_in_memory(short a);
size_t size_in_memory(int a);
size_t size_in_memory(long a);
size_t size_in_memory(long long a);
size_t size_in_memory(unsigned char a);
size_t size_in_memory(unsigned short a);
size_t size_in_memory(unsigned int a);
size_t size_in_memory(unsigned long a);
size_t size_in_memory(unsigned long long a);

// Absolute value
float         abs(float a);
double        abs(double a);
char          abs(char a);
short         abs(short a);
int           abs(int a);
long          abs(long a);
long long     abs(long long a);
constexpr unsigned char  abs(unsigned char a);
constexpr unsigned short abs(unsigned short a);
constexpr unsigned int   abs(unsigned int a);
constexpr unsigned long  abs(unsigned long a);
constexpr unsigned long long  abs(unsigned long long a);

// Pointer access
float const*          ptr(float const& a);
double const*         ptr(double const& a);
char const*           ptr(char const& a);
short const*          ptr(short const& a);
int const*            ptr(int const& a);
long const*           ptr(long const& a);
long long const*      ptr(long long const& a);
unsigned char const*  ptr(unsigned char const& a);
unsigned short const* ptr(unsigned short const& a);
unsigned int const*   ptr(unsigned int const& a);
unsigned long const*  ptr(unsigned long const& a);
unsigned long long const* ptr(unsigned long long const& a);

std::string str(float a);
std::string str(double a);
std::string str(char a);
std::string str(short a);
std::string str(int a);
std::string str(long a);
std::string str(long long a);
std::string str(unsigned char a);
std::string str(unsigned short a);
std::string str(unsigned int a);
std::string str(unsigned long a);
std::string str(unsigned long long a);


// Clamp x value between [x_min, x_max]
template <typename T1, typename T2> T1 clamp(T1 x, T2 x_min, T2 x_max);


std::string type_str(float );
std::string type_str(double);
std::string type_str(char);
std::string type_str(short);
std::string type_str(int);
std::string type_str(long);
std::string type_str(long long);
std::string type_str(unsigned char);
std::string type_str(unsigned short);
std::string type_str(unsigned int);
std::string type_str(unsigned long);
std::string type_str(unsigned long long);


template <typename T> struct cgp_trait{};
template <> struct cgp_trait<float> {
    static float one() { return 1.0f; }
    static float zero() { return 0.0f; }
};


}

// Template implementation

namespace cgp
{

template <typename T1, typename T2>
T1 clamp(T1 x, T2 x_min, T2 x_max)
{
    if( x<x_min )
        return x_min;
    if( x>x_max )
        return x_max;
    return x;
}

}
