	#include <iostream>
	#include <fstream>
	#include <vector>
	#include <string>
	#include <map>
	#include <deque>
	#include <list>
	#include "Utils.h"
	#include "FileHandler.h"
	#include "TransitionHandler.h"

	int main(int argc, char* argv[])
	{
		if (argc < 3)
		{
			std::cout << "Usage: <program> <input.csv> <output.csv>" << std::endl;
			return 1;
		}

		std::ifstream input(argv[1]);

		Automata automata = FileHandler::ReadCSV(argv[1]);

		CollapsedState startState = { automata.states[0].name, automata.states[0].isFinal, { automata.states[0] } };

		std::vector<std::string> args;
		for (auto arg : automata.args)
			if (arg != "eps")
				args.push_back(arg);

		std::map<State, std::vector<State>> statesToEpsClosure = TransitionHandler::ComputeEpsilonClosures(automata);

		std::vector<CollapsedState> newProcessedStates;
		std::vector<NewTransition> newTransitions;
		TransitionHandler::ProcessStatesAndTransitions(automata, startState, args, statesToEpsClosure, newProcessedStates, newTransitions);

		FileHandler::WriteCSV(argv[2], newProcessedStates, newTransitions, args);

	}