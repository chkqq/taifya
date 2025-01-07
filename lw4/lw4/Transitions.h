#pragma once
#include "State.h"

struct Transition
{
	State& from;
	State& to;
	std::string arg;
};

struct NewTransition
{
	State from;
	State to;
	std::string arg;
};