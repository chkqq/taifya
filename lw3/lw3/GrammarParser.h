#pragma once

#include <sstream>
#include "GrammarSide.h"
#include "Machine.h"

class GrammarParser
{
public:
    static dev::Machine ParseGrammarToMachine(std::istream& input, GrammarSide grammarSide);

private:

    static void ParseGrammarToMachine(dev::Machine& machine, std::istream& input, bool isLeftSideGrammar);
};