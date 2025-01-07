#pragma once
#include "TransitionTable.h"

std::vector<Transition> TransitionTable::GenerateTransitions(const std::vector<Pair>& moves, const std::unordered_map<std::string, std::string>& states, bool isLeft)
{
    std::vector<Transition> transitions;

    for (const auto& move : moves)
    {
        for (const auto& transition : move.value)
        {
            std::vector<std::string> splitTransition = Utils::Split(transition, " ");
            Transition transit;
            if (splitTransition.size() == 1)
            {
                transit.arg = splitTransition[0];
                if (isLeft)
                {
                    transit.from = states.at("finalState");
                    transit.to = states.at(move.key);
                }
                else
                {
                    transit.from = states.at(move.key);
                    transit.to = states.at("finalState");
                }
            }
            else
            {
                if (isLeft)
                {
                    transit.from = states.at(splitTransition[0]);
                    transit.to = states.at(move.key);
                    transit.arg = splitTransition[1];
                }
                else
                {
                    transit.from = states.at(move.key);
                    transit.to = states.at(splitTransition[1]);
                    transit.arg = splitTransition[0];
                }
            }
            transitions.push_back(transit);
        }
    }
    return transitions;
}