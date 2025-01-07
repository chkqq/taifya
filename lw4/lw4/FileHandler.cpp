#include "FileHandler.h"
#include <iostream>

Automata FileHandler::ReadCSV(const std::string& filePath)
{
    std::ifstream input(filePath);

    Automata automata;
    std::string line1, line2;

    getline(input, line1);
    getline(input, line2);
    auto statesLine = Utils::Split(line1, ";");
    auto namesLine = Utils::Split(line2, ";");

    for (size_t i = 1; i < statesLine.size(); i++)
    {
        bool isFinal = (statesLine[i] == "F");
        automata.states.push_back({ namesLine[i], isFinal });
    }

    while (getline(input, line1))
    {
        auto tokens = Utils::Split(line1, ";");
        if (tokens.empty()) continue;

        std::string arg = tokens[0];
        automata.args.push_back(arg);

        for (size_t i = 1; i < tokens.size(); i++)
        {
            auto targetStates = Utils::Split(tokens[i], ",");
            for (const auto& target : targetStates)
            {
                for (auto& state : automata.states)
                {
                    if (state.name == target)
                    {
                        automata.transitions.push_back({ automata.states[i - 1], state, arg });
                        break;
                    }
                }
            }
        }
    }

    input.close();
    return automata;
}

void FileHandler::WriteCSV
(
    const std::string& filePath, 
    const std::vector<CollapsedState>& newProcessedStates, 
    const std::vector<NewTransition>& newTransitions, 
    const std::vector<std::string>& args
)
{
    std::ofstream output(filePath);
    std::unordered_map<std::string, std::string> newNames;
    for (size_t i = 0; i < newProcessedStates.size(); i++)
    {
        newNames[newProcessedStates[i].name] = "s" + std::to_string(i);
    }

    output << ";";
    for (const auto& state : newProcessedStates)
    {
        if (state.isFinal)
        {
            output << "F";
        }
        output << ";";
    }
    output << "\n" << ";";

    for (const auto& state : newProcessedStates)
    {
        output << newNames[state.name] << ";";
    }
    output << "\n";

    for (const auto& arg : args)
    {
        output << arg << ";";
        for (const auto& state : newProcessedStates)
        {
            bool transitionFound = false;
            for (const auto& transition : newTransitions)
            {
                if (transition.from.name == state.name && transition.arg == arg)
                {
                    output << newNames[transition.to.name];
                    transitionFound = true;
                    break;
                }
            }
            if (!transitionFound)
            {
                output << "";
            }
            output << ";";
        }
        output << "\n";
    }

    
	for (auto name : newNames)
		std::cout << name.first << " -> " << name.second << std::endl;
}