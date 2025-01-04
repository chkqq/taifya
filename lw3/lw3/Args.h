#pragma once

#include <string>
#include "GrammarSide.h"

struct Args
{
    GrammarSide grammarSide;
    std::string inputFile;
    std::string outputFile;
};