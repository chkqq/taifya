#include "TransitionHandler.h"

std::map<State, std::vector<State>> TransitionHandler::ComputeEpsilonClosures(const Automata& automata) {
    std::map<State, std::vector<State>> statesToEpsClosure;
    std::deque<State> queue;
    std::vector<State> processedStates;

    for (const auto& state : automata.states) {
        std::vector<State> epsClosure = { state };
        statesToEpsClosure[state] = epsClosure;
        queue.push_back(state);

        while (!queue.empty()) {
            State tempState = queue.front();
            queue.pop_front();

            if (std::find(processedStates.begin(), processedStates.end(), tempState) != processedStates.end()) {
                continue;
            }

            processedStates.push_back(tempState);

            for (const auto& transition : automata.transitions) {
                if (transition.from == tempState && transition.arg == "eps") {
                    if (std::find(statesToEpsClosure[state].begin(), statesToEpsClosure[state].end(), transition.to) == statesToEpsClosure[state].end()) {
                        statesToEpsClosure[state].push_back(transition.to);
                        queue.push_back(transition.to);
                    }
                }
            }
        }

        processedStates.clear();
    }

    return statesToEpsClosure;
}

void TransitionHandler::ProcessStatesAndTransitions(
    const Automata& automata,
    const CollapsedState& startState,
    const std::vector<std::string>& args,
    const std::map<State, std::vector<State>>& statesToEpsClosure,
    std::vector<CollapsedState>& newProcessedStates,
    std::vector<NewTransition>& newTransitions
) {
    std::deque<CollapsedState> newQueue;
    newQueue.push_back(startState);

    while (!newQueue.empty())
    {
        CollapsedState state = newQueue[0];
        newQueue.pop_front();
        newProcessedStates.push_back(state);
        for (auto arg : args)
        {
            std::list<State> reachableStates;
            for (int i = 0; i < state.states.size(); i++)
            {
                for (auto transition : automata.transitions)
                    if (std::find(statesToEpsClosure.at(state.states[i]).begin(),
                        statesToEpsClosure.at(state.states[i]).end(),
                        transition.from) != statesToEpsClosure.at(state.states[i]).end()
                        && transition.arg == arg)
                        reachableStates.push_back(transition.to);
            }

            if (reachableStates.empty())
                continue;

            reachableStates.sort();
            reachableStates.unique();

            CollapsedState st = { reachableStates.front().name, reachableStates.front().isFinal, { reachableStates.front() } };
            reachableStates.pop_front();

            while (!reachableStates.empty())
            {
                st.name += "," + reachableStates.front().name;
                if (reachableStates.front().isFinal)
                    st.isFinal = true;

                st.states.push_back(reachableStates.front());

                reachableStates.pop_front();
            }

            if (std::find(newProcessedStates.begin(), newProcessedStates.end(), st) == newProcessedStates.end())
                newQueue.push_back(st);

            newTransitions.push_back(NewTransition({ { state.name, state.isFinal }, { st.name, st.isFinal }, arg }));
        }
    }
}
