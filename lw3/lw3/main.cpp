#include "Parser.h"
#include "Pair.h"
#include "FileHandler.h"
#include "StateMapper.h"
#include "TransitionTable.h"
#include "Utils.h"

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        std::cout << "Usage: <program> <grammar.txt> <output.csv>" << std::endl;
        return 1;
    }

    std::vector<Pair> moves = Parser::ParseGrammar(argv[1]);
    bool isLeft = Utils::Any(moves[0].value);
    std::unordered_map<std::string, std::string> states = StateMapper::MapStates(moves, isLeft);
    std::vector<Transition> transitions = TransitionTable::GenerateTransitions(moves, states, isLeft);
    FileHandler::WriteCSV(argv[2], states, transitions);

    for (const auto& state : states)
        std::cout << state.first << " -> " << state.second << std::endl;

    return 0;
}
