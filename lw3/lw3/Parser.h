#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <string>
#include "Pair.h"
#include "Utils.h"
#include <fstream>

class Parser
{
public:
    static std::vector<Pair> ParseGrammar(const std::string& filename);
};

#endif 