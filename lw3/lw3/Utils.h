#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include <regex>
    

class Utils
{
public:
    static std::string Trim(const std::string& str);
    static std::vector<std::string> Split(const std::string& str, const std::string& del);
    static bool Any(const std::vector<std::string>& vector);
};

#endif 