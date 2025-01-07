#ifndef STATEMAPPER_H
#define STATEMAPPER_H

#include <unordered_map>
#include <string>
#include "Pair.h"

class StateMapper
{
public:
    static std::unordered_map<std::string, std::string> MapStates(const std::vector<Pair>& moves, bool isLeft);
};

#endif 