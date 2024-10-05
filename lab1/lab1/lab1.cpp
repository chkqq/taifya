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

bool CompareTransition(const Transition& move1, const Transition& move2) {
    return move1.nextState == move2.nextState && move1.output == move2.output;
}

std::vector<Transition>::iterator FindTransitonInVector(std::vector<Transition>& vector, const Transition& move) {
    for (int i = 0; i < vector.size(); ++i) {
        if (CompareTransition(vector[i], move))
            return vector.begin() + i;
    }
    return vector.end();
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

TransitionTable ParseMealyTable(std::ifstream& input, StateList& inputs, StateList& states) {
    std::string line;
    getline(input, line);
    states = ParseCSVLine(line.substr(1));

    TransitionTable table;
    while (getline(input, line)) {
        inputs.push_back(line.substr(0, line.find(';')));
        line = line.substr(line.find(';') + 1);

        std::stringstream ss(line);
        std::string cell;
        std::vector<Transition> row;

        while (std::getline(ss, cell, ';')) {
            auto slashPos = cell.find('/');
            row.push_back({ cell.substr(0, slashPos), cell.substr(slashPos + 1) });
        }
        table.push_back(row);
    }
    return table;
}

void ConvertMealyToMoore(const std::string& inputFile, const std::string& outputFile) {
    std::ifstream input(inputFile);
    StateList inputs, states;
    TransitionTable table = ParseMealyTable(input, inputs, states);
    input.close();

    std::vector<Transition> distinctTransitions;
    std::unordered_map<std::string, int> stateMapping;

    for (const auto& row : table) {
        for (const auto& transition : row) {
            // Находим уникальные переходы по состояниям и выходам
            auto it = std::find_if(distinctTransitions.begin(), distinctTransitions.end(),
                [&](const Transition& t) { return CompareTransition(t, transition); });

            if (it == distinctTransitions.end()) {
                distinctTransitions.push_back(transition);
                stateMapping[transition.nextState + "/" + transition.output] = distinctTransitions.size() - 1;
            }
        }
    }

    std::ofstream output(outputFile);
    output << ";";
    for (const auto& transitionPair : distinctTransitions) {
        output << transitionPair.output << ";";
    }

    output << "\n;";

    for (size_t i = 0; i < distinctTransitions.size(); ++i) {
        output << "S" << i << ";";
    }
    output << "\n";

    for (size_t i = 0; i < inputs.size(); i++) {
        output << inputs[i] << ";";
        for (size_t o = 0; o < distinctTransitions.size(); ++o) {
            const Transition& tempMove = table[i][std::find(inputs.begin(), inputs.end(), distinctTransitions[o].nextState) - inputs.begin()];
            output << "S" << (FindTransitonInVector(distinctTransitions, tempMove) - distinctTransitions.begin()) << ";";
        }
        output << "\n";
    }
    output.close();
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
        std::cerr << "Invalid mode. Use 'mealy-to-moore' or 'moore-to-mealy'.\n";
        return 1;
    }
    return 0;
}
