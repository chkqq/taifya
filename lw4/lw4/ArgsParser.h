#pragma once

#include <string>
#include "Args.h"

class ArgsParser
{
public:
    static bool Parse(int argc, char* argv[], Args& args)
    {
        if (argc != 3)
        {
            return false;
        }

        args.inputFile = argv[1];
        args.outputFile = argv[2];
        return true;
    }
};
