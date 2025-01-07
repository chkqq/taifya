#include "Utils.h"

std::string Utils::Trim(const std::string& str)
{
    return str.substr(str.find_first_not_of(" "), str.find_last_not_of(" ") - str.find_first_not_of(" ") + 1);
}

std::vector<std::string> Utils::Split(const std::string& str, const std::string& del)
{
    std::vector<std::string> tempVector;
    std::string tempStr = str;
    while (tempStr.find(del) != std::string::npos)
    {
        tempVector.push_back(Trim(tempStr.substr(0, tempStr.find(del))));
        tempStr.erase(0, tempStr.find(del) + del.size());
    }
    tempVector.push_back(Trim(tempStr));
    return tempVector;
}

bool Utils::Any(const std::vector<std::string>& vector)
{
    std::regex r(R"(<(?:\d|\w)+> \S)");
    for (const auto& str : vector)
        if (std::regex_match(str, r))
            return true;
    return false;
}