#include "Utils.h"


std::string Utils::Trim(std::string string)
{
	if (!string.empty())
		return string.substr(string.find_first_not_of(" "), string.find_last_not_of(" ") - string.find_first_not_of(" ") + 1);
	else
		return string;
};

std::vector<std::string> Utils::Split(std::string string, std::string del)
{
	std::vector<std::string> tempVector;
	while (string.find(del) != std::string::npos)
	{
		tempVector.push_back(Trim(string.substr(0, string.find(del))));
		string.erase(0, string.find(del) + del.size());
	}
	tempVector.push_back(Trim(string));
	return tempVector;
};