#include "string.hpp"
#include <algorithm>

namespace cgp
{

std::string str_zero_fill(const std::string& input, size_t number_of_zero)
{
    std::string new_string = "";
    const size_t N = input.size();
    for(size_t k=N; k<number_of_zero; ++k)
        new_string += "0";
    new_string += input;
    return new_string;
}

std::string const& str(std::string const& s)
{
    return s;
}

std::string str(char const* s)
{
    return std::string(s);
}

bool is_equal(std::string const& a, std::string const& b)
{
    return a==b;
}
bool is_equal(std::string const& a, char const* b)
{
    return is_equal(a, str(b));
}
bool is_equal(char const* a, std::string const& b)
{
    return is_equal(str(a), b);
}

std::string type_str(std::string const& )
{
    return "string";
}

std::string str_lower(std::string const& s)
{
    std::string data = s;
    std::transform(data.begin(), data.end(), data.begin(),
        [](unsigned char c) { return std::tolower(c); });

    return data;
}
std::string str_upper(std::string const& s)
{
    std::string data = s;
    std::transform(data.begin(), data.end(), data.begin(),
        [](unsigned char c) { return std::toupper(c); });

    return data;
}

}
