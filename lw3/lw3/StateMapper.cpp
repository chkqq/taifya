#include "StateMapper.h"

std::unordered_map<std::string, std::string> StateMapper::MapStates(const std::vector<Pair>& moves, bool isLeft)
{
    std::unordered_map<std::string, std::string> states;

    if (isLeft)
    {
        states["finalState"] = "q0";
        for (size_t i = 1; i < moves.size(); i++)
            states[moves[i].key] = "q" + std::to_string(i);
        states[moves[0].key] = "q" + std::to_string(states.size());
    }
    else
    {
        for (size_t i = 0; i < moves.size(); i++)
            states[moves[i].key] = "q" + std::to_string(i);
        states["finalState"] = "q" + std::to_string(states.size());
    }

    return states;
}