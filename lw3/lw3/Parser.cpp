#include "Parser.h"

std::vector<Pair> Parser::ParseGrammar(const std::string& filename)
{
    std::ifstream input(filename);
    std::vector<Pair> moves;
    std::string line, temp;

    getline(input, temp);
    while (!input.eof())
    {
        line = temp;
        if (!input.eof())
        {
            getline(input, temp);
        }

        while ((!input.eof()) && (temp.find("->") == std::string::npos))
        {
            line += temp;
            getline(input, temp);
        }

        std::vector<std::string> tempVector = Utils::Split(line, "->");
        moves.push_back({ tempVector[0], Utils::Split(tempVector[1], "|") });
    }
    return moves;
}