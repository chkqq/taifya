#pragma once
#include <vector>
#include "Transitions.h"

struct Automata
{
	std::vector<State> states;
	std::vector<std::string> args;
	std::vector<Transition> transitions;
};