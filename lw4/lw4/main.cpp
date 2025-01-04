#include <iostream>
#include "ArgsParser.h"
#include "DSMConverter.h"
#include "MachineSaver.h"
#include "CSVTextParser.h"
#include <fstream>

bool PrepareStreams(std::ifstream& input, std::ofstream& output, const Args& args)
{
    input.open(args.inputFile);

    if (!input.is_open())
    {
        std::cerr << "Input file couldn't be opened: " << args.inputFile << std::endl;
        return false;
    }

    output.open(args.outputFile);

    if (!output.is_open())
    {
        std::cerr << "Output file couldn't be opened: " << args.outputFile << std::endl;
        return false;
    }

    return true;
}

int main(int argc, char* argv[])
{
    Args args;
    if (!ArgsParser::Parse(argc, argv, args)) // Учитывается, что ArgsParser::Parse возвращает bool
    {
        std::cerr << "Wrong usage. Example: .exe left-type-grammar.txt output.csv" << std::endl;
        return 1;
    }

    std::ifstream input;
    std::ofstream output;
    if (!PrepareStreams(input, output, args))
    {
        return 1;
    }

    DSMConverter dsmConverter; // Используем объект вместо `new` для автоматического управления памятью

    try
    {
        MachineSaver::Save(output, dsmConverter.ConvertToDSM(CSVTextParser::GetMachine(input)));
    }
    catch (const std::exception& e)
    {
        std::cerr << "An error occurred during conversion: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
