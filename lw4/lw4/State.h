#pragma once
#include <string>

struct State
{
	bool operator==(const State& state) const
	{
		return this->name == state.name;
	};
	bool operator<(const State& state) const
	{
		return this->name < state.name;
	};
	std::string name;
	bool isFinal;
};