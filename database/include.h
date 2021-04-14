/**
 * File: include.h
 * Author: Mark Minkoff
 * Functionality: Instantiates useful standard libaries and helper functions
 * This file is included in all other files.
 * 
 * */

#ifndef INCLUDE_H_
#define INCLUDE_H_

#include <algorithm>
#include <cmath>
#include <ctype.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <numeric>
#include <map>
#include <queue>
#include <regex>
#include <sstream>
#include <string>
#include <stack>
#include <utility>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

namespace fs = std::filesystem;

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

// Rounds a floating point number by a set number of significant digits (in theory)
// Does not work on Ubuntu
static float _roundFloat(float num, unsigned int sig_figs) 
{
    float power_of_10 = std::pow(10, sig_figs);

    float res = std::round(num * power_of_10)  / power_of_10;

    return res;
}

/** Trims all white spaces frm the right side of a string */
static std::string _trimEnd(std::string source) 
{
    // Trim from the end iterator to the last iterator of a white space   
    source.erase(std::find_if(source.rbegin(), source.rend(), [](char c) 
    {
        return !std::isspace(static_cast<unsigned char>(c));
    }).base(), source.end());
    return source;
}

/** Trims all white spaces frm the left side of a string */
static std::string _trimStart(std::string source) 
{
    // Trim from the start iterator to the last iterator of a white space
    source.erase(source.begin(), std::find_if(source.begin(), source.end(), [](char c) 
    {
        return !std::isspace(static_cast<unsigned char>(c));
    }));
    return source;
}

/** Trim white spaces from left and right side of a string */
static std::string _trim(std::string source) 
{
    return _trimEnd(_trimStart(source));
}

/** Checks if an operator is valid */
static bool _isValidOperator(const std::string& op)
{
    // Return true if operator is valid
    // otherwise, return false
    if (
        op == "="  || 
        op == "!=" || 
        op == "<"  || 
        op == "<=" || 
        op == ">"  || 
        op == ">="
    ) return true;
    return false;
}

// Converts a character to uppercase
static char _toUpper(const char& c)
{
    char res = c;
    // If a character is in range of 'a' to 'z' in ASCII
    // convert to lowercase
    if (res >= 'a' && res <= 'z') {
        res = res - 32;
    }
    return res;
}

/**  * Split a string by a delimiter and return a vector of strings
 * @param string The string being split
 * @param char The delimiter to split by
 * @return vector<string> The split string */
static std::vector<std::string> _split(const std::string& s, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter))
    {
        tokens.push_back(token);
    }
    return tokens;
}

/**  * Check if a parenthesis are balanced in a string. 
 *  ! Credit goes to chammika at leetcode.com
 * * https://leetcode.com/problems/valid-parentheses/discuss/9252/2ms-C%2B%2B-sloution
 * @param string The string being split
 * @return bool */
static bool _isBalancedParenthesis(std::string s)
{
    std::stack<char> p;
    for (char& c : s) {
        switch (c) {
            case '(': 
            case '{': 
            case '[': p.push(c); break;
            case ')': if (p.empty() || p.top()!='(') return false; else p.pop(); break;
            case '}': if (p.empty() || p.top()!='{') return false; else p.pop(); break;
            case ']': if (p.empty() || p.top()!='[') return false; else p.pop(); break;
            default: ; 
        }
    }
    return p.empty();
}

#endif // INCLUDE_H_
