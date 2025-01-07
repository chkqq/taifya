#ifndef TRANSITIONHANDLER_H
#define TRANSITIONHANDLER_H

#include <map>
#include <vector>
#include <deque>
#include <list>
#include "Utils.h"
#include "Automata.h"
#include "CollapsedState.h"


class TransitionHandler {
public:
    static std::map<State, std::vector<State>> ComputeEpsilonClosures(const Automata& automata);

    static void ProcessStatesAndTransitions(
        const Automata& automata,
        const CollapsedState& startState,
        const std::vector<std::string>& args,
        const std::map<State, std::vector<State>>& statesToEpsClosure,
        std::vector<CollapsedState>& newProcessedStates,
        std::vector<NewTransition>& newTransitions
    );
};

#endif
