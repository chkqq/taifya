#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <string>
#include <vector>
#include <unordered_map>
#include <list>
#include <iostream>
#include <fstream>
#include "Transition.h"

class FileHandler
{
public:
    static void WriteCSV(const std::string& filename, const std::unordered_map<std::string, std::string>& states, const std::vector<Transition>& transitions);
private:
    static std::list<std::string> GetUniqueArgs(const std::vector<Transition>& transitions);
};

#endif