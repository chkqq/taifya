#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>


class Utils
{
public:
    static std::string Trim(std::string string);
    static std::vector<std::string> Split(std::string string, std::string del);
};

#endif 