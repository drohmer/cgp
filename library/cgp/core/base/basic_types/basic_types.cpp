#include "basic_types.hpp"

#include <iostream>
#include <cmath>

namespace cgp
{

bool is_equal(float a, float b)
{
    return std::abs(a-b) <= 1e-5f;
}
bool is_equal(double a, double b)
{
    return std::abs(a-b) <= 1e-7f;
}


bool is_equal(float a, double b) 
{
    return std::abs(a - float(b)) <= 1e-5f;
}
bool is_equal(double a, float b)
{
    return std::abs(float(a) - b) <= 1e-5f;
}



constexpr size_t size(float )          { return 1; }
constexpr size_t size(double )         { return 1; }
constexpr size_t size(char )           { return 1; }
constexpr size_t size(short )          { return 1; }
constexpr size_t size(int )            { return 1; }
constexpr size_t size(long )           { return 1; }
constexpr size_t size(long long)       { return 1; }
constexpr size_t size(unsigned char )  { return 1; }
constexpr size_t size(unsigned short ) { return 1; }
constexpr size_t size(unsigned int )   { return 1; }
constexpr size_t size(unsigned long )  { return 1; }
constexpr size_t size(unsigned long long) { return 1; }


size_t size_in_memory(float a)          { return sizeof(a); }
size_t size_in_memory(double a)         { return sizeof(a); }
size_t size_in_memory(char a)           { return sizeof(a); }
size_t size_in_memory(short a)          { return sizeof(a); }
size_t size_in_memory(int a)            { return sizeof(a); }
size_t size_in_memory(long a)           { return sizeof(a); }
size_t size_in_memory(long long a)      { return sizeof(a); }
size_t size_in_memory(unsigned char a)  { return sizeof(a); }
size_t size_in_memory(unsigned short a) { return sizeof(a); }
size_t size_in_memory(unsigned int a)   { return sizeof(a); }
size_t size_in_memory(unsigned long a)  { return sizeof(a); }
size_t size_in_memory(unsigned long long a) { return sizeof(a); }


float         abs(float a)  { return std::abs(a); }
double        abs(double a) { return std::abs(a); }
char          abs(char a)   { return a > 0 ? a : -a ; }
short         abs(short a)  { return a > 0 ? a : -a; }
int           abs(int a)    { return a > 0 ? a : -a; }
long          abs(long a)   { return a > 0 ? a : -a; }
long long     abs(long long a) { return a > 0 ? a : -a; }
constexpr unsigned char  abs(unsigned char a)  { return a; }
constexpr unsigned short abs(unsigned short a) { return a; }
constexpr unsigned int   abs(unsigned int a)   { return a; }
constexpr unsigned long  abs(unsigned long a)  { return a; }
constexpr unsigned long long  abs(unsigned long long a) { return a; }


float const* ptr(float const& a)                   { return &a; }
double const* ptr(double const& a)                 { return &a; }
char const* ptr(char const& a)                     { return &a; }
short const* ptr(short const& a)                   { return &a; }
int const* ptr(int const& a)                       { return &a; }
long const* ptr(long const& a)                     { return &a; }
long long const* ptr(long long const& a)           { return &a; }
unsigned char const* ptr(unsigned char const& a)   { return &a; }
unsigned short const* ptr(unsigned short const& a) { return &a; }
unsigned int const* ptr(unsigned int const& a)     { return &a; }
unsigned long const* ptr(unsigned long const& a)   { return &a; }
unsigned long long const* ptr(unsigned long long const& a) { return &a; }


std::string str(float a)          { return std::to_string(a); }
std::string str(double a)         { return std::to_string(a); }
std::string str(char a)           { return std::string(1,a); }
std::string str(short a)          { return std::to_string(a); }
std::string str(int a)            { return std::to_string(a); }
std::string str(long a)           { return std::to_string(a); }
std::string str(long long a)      { return std::to_string(a); }
std::string str(unsigned char a)  { return std::to_string(a); }
std::string str(unsigned short a) { return std::to_string(a); }
std::string str(unsigned int a)   { return std::to_string(a); }
std::string str(unsigned long a)  { return std::to_string(a); }
std::string str(unsigned long long a) { return std::to_string(a); }



std::string type_str(float)              { return "float"; }
std::string type_str(double)             { return "double"; }
std::string type_str(char)               { return "char"; }
std::string type_str(short)              { return "short"; }
std::string type_str(int)                { return "int"; }
std::string type_str(long)               { return "long"; }
std::string type_str(long long)          { return "long long"; }
std::string type_str(unsigned char)      { return "unsigned char"; }
std::string type_str(unsigned short)     { return "unsigned short"; }
std::string type_str(unsigned int)       { return "unsigned int"; }
std::string type_str(unsigned long)      { return "unsigned long"; }
std::string type_str(unsigned long long) { return "unsigned long long"; }

}
