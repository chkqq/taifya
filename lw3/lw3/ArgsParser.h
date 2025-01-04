#include <memory> // std::unique_ptr
#include "Args.h"

class ArgsParser
{
public:
    static std::unique_ptr<Args> Parse(int argc, char* argv[])
    {
        auto args = std::make_unique<Args>();
        const std::string LEFT_GRAMMAR_TYPE_COMMAND = "left";
        const std::string RIGHT_GRAMMAR_TYPE_COMMAND = "right";

        if (argc != 4)
        {
            return nullptr;
        }

        if (argv[1] == LEFT_GRAMMAR_TYPE_COMMAND)
        {
            args->grammarSide = GrammarSide::LEFT;
        }
        else if (argv[1] == RIGHT_GRAMMAR_TYPE_COMMAND)
        {
            args->grammarSide = GrammarSide::RIGHT;
        }
        else
        {
            return nullptr;
        }

        args->inputFile = argv[2];
        args->outputFile = argv[3];
        return args;
    }
};
