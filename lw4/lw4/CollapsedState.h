#pragma once
#include <vector>
#include "State.h"

struct CollapsedState
{
	bool operator==(const CollapsedState& state) const
	{
		return this->name == state.name;
	};

	std::string name;
	bool isFinal;
	std::vector<State> states;
};