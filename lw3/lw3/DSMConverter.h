#pragma once

#include "Machine.h"
#include "GrammarSide.h"

class DSMConverter
{
public:
    static client::Machine ConvertToDSM(const dev::Machine& originMachine, GrammarSide grammarSide);
};
