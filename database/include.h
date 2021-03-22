#ifndef INCLUDE_H_
#define INCLUDE_H_

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <filesystem>
#include <map>
#include <stack>
#include <utility>
#include <algorithm>
#include <queue>
#include <ctype.h>
#include <numeric>
#include <variant>
#include <cmath>

// Converts a string to upper case
static std::string _toUpper(std::string str)
{
    // Create a temp string of size 'str'
    std::string up;
    up.reserve(str.size());

    // Convert each letter of 'str' to uppercase and append to 'up'
    for (auto& c : str) up += ::toupper(c);
    
    // Return the uppercase string
    return up;
}

// Joins a vector of strings by a delimeter
static std::string _join(std::vector<std::string> stringVector, std::string delim = "")
{
    return std::accumulate(stringVector.begin(), stringVector.end(), delim);
}
// Returns a subvector
template<typename T>
static auto _subVector(const std::vector<T>& vec, typename std::vector<T>::const_iterator left, typename std::vector<T>::const_iterator right) -> std::vector<T>
{
    // Return a vector in range of a left iterator to a right iterator
    return std::vector<T>(left, right);
}

// Rounds a floating point number by a set number of significant digits
static float _roundFloat(float num, unsigned int sig_figs) 
{
    float power_of_10 = std::pow(10, sig_figs);

    float res = std::round(num * power_of_10)  / power_of_10;

    return res;
}

#endif // INCLUDE_H_
