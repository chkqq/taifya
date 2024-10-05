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

void ConvertMealyToMoore(const std::string& inputFile, const std::string& outputFile) {
    std::ifstream input(inputFile);
    std::string tempStr;

    getline(input, tempStr);
    tempStr = tempStr.substr(1); 

    std::vector<std::string> listOfChars;
    while (tempStr.find(";") != std::string::npos) {
        listOfChars.push_back(tempStr.substr(0, tempStr.find(";")));
        tempStr = tempStr.substr(tempStr.find(";") + 1);
    }
    listOfChars.push_back(tempStr); 

    std::vector<std::vector<Transition>> table;
    std::vector<std::string> inputs; 
    std::vector<Transition> distinctTransitions; 

    while (getline(input, tempStr)) {
        std::vector<Transition> row;
        inputs.push_back(tempStr.substr(0, tempStr.find(";")));
        tempStr = tempStr.substr(tempStr.find(";") + 1);

        std::string tempMove;
        Transition move;

        tempStr += ";";
        while (tempStr.find(";") != std::string::npos) {
            tempMove = tempStr.substr(0, tempStr.find(";"));
            tempStr = tempStr.substr(tempStr.find(";") + 1);

            move.output = tempMove.substr(tempMove.find('/') + 1);
            move.nextState = tempMove.substr(0, tempMove.find('/'));

            row.push_back(move);

            if (FindTransitonInVector(distinctTransitions, move) == distinctTransitions.end()) {
                distinctTransitions.push_back(move);
            }
        }
        table.push_back(row);
    }
    input.close();

    std::vector<Transition> sortedQ;
    while (!distinctTransitions.empty()) {
        Transition minMove = distinctTransitions[0];
        for (const auto& move : distinctTransitions) {
            if (move.nextState < minMove.nextState) {
                minMove = move;
            }
        }
        sortedQ.push_back(minMove);
        distinctTransitions.erase(FindTransitonInVector(distinctTransitions, minMove));
    }

    if (sortedQ[0].nextState != listOfChars[0]) {
        distinctTransitions.push_back({ "-", listOfChars[0] });
        distinctTransitions.insert(distinctTransitions.end(), sortedQ.begin(), sortedQ.end());
    }
    else {
        distinctTransitions = sortedQ;
    }

    std::ofstream output(outputFile);

    output << ";";
    for (const auto& move : distinctTransitions) {
        output << move.output << ";";
    }
    output << "\n;";

    for (size_t i = 0; i < distinctTransitions.size(); ++i) {
        output << "q" << i << ";";
    }
    output << "\n";

    for (size_t i = 0; i < inputs.size(); ++i) {
        output << inputs[i] << ";";
        for (size_t o = 0; o < distinctTransitions.size(); ++o) {
            const Transition& tempMove = table[i][std::find(listOfChars.begin(), listOfChars.end(), distinctTransitions[o].nextState) - listOfChars.begin()];
            output << "q" << (FindTransitonInVector(distinctTransitions, tempMove) - distinctTransitions.begin()) << ";";
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
