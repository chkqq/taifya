#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>

struct Transition {
    std::string nextState;
    std::string output;
};

using TransitionTable = std::vector<std::vector<Transition>>;
using StateList = std::vector<std::string>;

struct StateTransition {
    std::string output;
    std::string state;
};

bool AreTransitionsEqual(const StateTransition& transition1, const StateTransition& transition2) {
    return transition1.state == transition2.state && transition1.output == transition2.output;
}

std::vector<StateTransition>::iterator FindTransitionInVector(std::vector<StateTransition>& transitions, const StateTransition& transition) {
    return std::find_if(transitions.begin(), transitions.end(), [&](const StateTransition& t) {
        return AreTransitionsEqual(t, transition);
        });
}

int FindTransitionIndexInVector(const std::vector<StateTransition>& transitions, const StateTransition& transition) {
    for (size_t i = 0; i < transitions.size(); ++i) {
        if (AreTransitionsEqual(transitions[i], transition)) {
            return i;
        }
    }
    return -1;
}

StateList ParseCSVLine(const std::string& line) {
    StateList elements;
    std::stringstream ss(line);
    std::string item;
    while (std::getline(ss, item, ';')) {
        elements.push_back(item);
    }
    return elements;
}

void ReadMealyInputFile(const std::string& inputFile, StateList& inputStates, std::vector<std::string>& inputs, std::vector<std::vector<StateTransition>>& transitionTable, std::vector<StateTransition>& uniqueTransitions) {
    std::ifstream input(inputFile);
    std::string headerLine;
    getline(input, headerLine);
    inputStates = ParseCSVLine(headerLine.substr(1));

    std::string line;
    while (getline(input, line)) {
        std::vector<StateTransition> transitions;
        inputs.push_back(line.substr(0, line.find(';')));
        line = line.substr(line.find(';') + 1);

        std::string transitionStr;
        StateTransition transition;
        line += ";";

        while (line.find(';') != std::string::npos) {
            transitionStr = line.substr(0, line.find(';'));
            line = line.substr(line.find(';') + 1);

            transition.output = transitionStr.substr(transitionStr.find('/') + 1);
            transition.state = transitionStr.substr(0, transitionStr.find('/'));

            transitions.push_back(transition);

            if (FindTransitionInVector(uniqueTransitions, transition) == uniqueTransitions.end()) {
                uniqueTransitions.push_back(transition);
            }
        }
        transitionTable.push_back(transitions);
    }
    input.close();
}

void WriteMooreOutputFile(const std::string& outputFile, const std::vector<StateTransition>& uniqueTransitions, const std::vector<std::string>& inputs, const std::vector<std::vector<StateTransition>>& transitionTable, const StateList& inputStates) {
    std::ofstream output(outputFile);

    output << ";";
    for (const auto& transition : uniqueTransitions) {
        output << transition.output << ";";
    }
    output << "\n;";

    for (size_t i = 0; i < uniqueTransitions.size(); ++i) {
        output << "q" << i << ";";
    }
    output << "\n";

    for (size_t i = 0; i < inputs.size(); ++i) {
        output << inputs[i] << ";";
        for (const auto& uniqueTransition : uniqueTransitions) {
            StateTransition tempTransition = transitionTable[i][std::find(inputStates.begin(), inputStates.end(), uniqueTransition.state) - inputStates.begin()];
            int transitionIndex = FindTransitionIndexInVector(uniqueTransitions, tempTransition);
            if (transitionIndex != -1) {
                output << "q" << transitionIndex << ";";
            }
            else {
                output << ";";
            }
        }
        output << "\n";
    }

    output.close();
}

void SortAndAdjustTransitions(std::vector<StateTransition>& uniqueTransitions, const StateList& inputStates) {
    std::sort(uniqueTransitions.begin(), uniqueTransitions.end(), [](const StateTransition& a, const StateTransition& b) {
        return a.state < b.state;
        });

    if (uniqueTransitions[0].state != inputStates[0]) {
        uniqueTransitions.insert(uniqueTransitions.begin(), { "-", inputStates[0] });
    }
}

void ConvertMealyToMoore(const std::string& inputFile, const std::string& outputFile) {
    StateList inputStates;
    std::vector<std::string> inputs;
    std::vector<std::vector<StateTransition>> transitionTable;
    std::vector<StateTransition> uniqueTransitions;

    ReadMealyInputFile(inputFile, inputStates, inputs, transitionTable, uniqueTransitions);
    SortAndAdjustTransitions(uniqueTransitions, inputStates);
    WriteMooreOutputFile(outputFile, uniqueTransitions, inputs, transitionTable, inputStates);
}

void ConvertMooreToMealy(const std::string& inputFile, const std::string& outputFile) {
    std::ifstream input(inputFile);
    std::ofstream output(outputFile);

    std::string line;
    getline(input, line);
    StateList outputs = ParseCSVLine(line.substr(1));

    getline(input, line);
    output << line << "\n";
    StateList states = ParseCSVLine(line.substr(1));

    std::unordered_map<std::string, std::string> stateOutputMap;
    for (size_t i = 0; i < states.size(); ++i) {
        stateOutputMap[states[i]] = outputs[i];
    }

    while (getline(input, line)) {
        output << line.substr(0, line.find(';')) << ";";
        line = line.substr(line.find(';') + 1);

        StateList transitions = ParseCSVLine(line);
        for (const auto& state : transitions) {
            output << state << "/" << stateOutputMap[state] << ";";
        }
        output << "\n";
    }
    input.close();
    output.close();
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " <mode> <input file> <output file>\n";
        return 1;
    }

    std::string mode = argv[1];
    if (mode == "1") {
        ConvertMealyToMoore(argv[2], argv[3]);
    }
    else if (mode == "2") {
        ConvertMooreToMealy(argv[2], argv[3]);
    }
    else {
        std::cerr << "Invalid mode. Use '1' for Mealy-to-Moore or '2' for Moore-to-Mealy.\n";
        return 1;
    }
    return 0;
}
