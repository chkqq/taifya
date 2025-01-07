#ifndef TRANSITIONTABLE_H
#define TRANSITIONTABLE_H


#include <vector>
#include <unordered_map>
#include "Pair.h"
#include "Transition.h"
#include "Utils.h"

class TransitionTable
{
public:
    static std::vector<Transition> GenerateTransitions(const std::vector<Pair>& moves, const std::unordered_map<std::string, std::string>& states, bool isLeft);
};

#endif 