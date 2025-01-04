#include <iostream>
#include "ArgsParser.h"
#include "GrammarParser.h"
#include "DSMConverter.h"
#include "MachineSaver.h"
#include <fstream>

bool PrepareStreams(std::ifstream& input, std::ofstream& output, const Args& args)
{
    input.open(args.inputFile);

    if (!input.is_open())
    {
        std::cout << "Input file couldn't be opened" << std::endl;
        return false;
    }

    output.open(args.outputFile);

    if (!output.is_open())
    {
        std::cout << "Output file couldn't be opened" << std::endl;
        return false;
    }

    return true;
}

int main(int argc, char* argv[])
{
    auto args = ArgsParser::Parse(argc, argv);
    if (!args)
    {
        std::cout << "Wrong usage. Example: .exe left left-type-grammar.txt output.csv" << std::endl;
        return 1;
    }

    std::ifstream input;
    std::ofstream output;
    if (!PrepareStreams(input, output, *args))
    {
        return 1;
    }

    auto machine = DSMConverter::ConvertToDSM(GrammarParser::ParseGrammarToMachine(input, args->grammarSide), args->grammarSide);

    MachineSaver::Save(output, machine);

    return 0;
}
