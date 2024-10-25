#include <fstream>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>

struct Move {
    std::string y;
    std::string s;
};

std::vector<std::string> FillingList(std::ifstream& input) {
    std::vector<std::string> list;
    std::string tempStr;
    getline(input, tempStr);
    tempStr = tempStr.substr(1, tempStr.size() - 1);
    while (tempStr.find(";") != std::string::npos) {
        list.push_back(tempStr.substr(0, tempStr.find(";")));
        tempStr = tempStr.substr(tempStr.find(";") + 1, tempStr.size() - tempStr.find(";"));
    }
    list.push_back(tempStr);
    return list;
}

void WritePositions(std::ofstream& output, const std::unordered_map<std::string, std::string>& moves) {
    int index = 1;
    int temp = 0;
    for (const auto& pair : moves) {
        if (stoi(pair.second) >= index) {
            output << ";" << "F" << std::stoi(pair.second) - 1;
            index++;
        }
        temp++;
    }
    output << std::endl;
}

void WriteListOfQ(std::ofstream& output, const std::unordered_map<std::string, std::string>& moves, const std::vector<std::string>& listOfQ) {
    int index = 1;
    int temp = 0;
    for (const auto& pair : moves) {
        if (std::stoi(pair.second) >= index) {
            output << ";" << listOfQ[temp];
            index++;
        }
        temp++;
    }
    output << std::endl;
}


std::unordered_map<std::string, std::string> ReWriteMoves(
    std::unordered_map<std::string, std::string>& lastMoves,
    const std::vector<std::string>& listOfS,
    const std::vector<std::vector<std::string>>& table
) {
    std::unordered_map<std::string, std::string> newMoves;
    int index = 1;
    for (int i = 0; i < table.size(); i++) {
        bool found = false;
        for (int o = 0; o < i; o++) {
            if (table[i] == table[o] && lastMoves[listOfS[i]] == lastMoves[listOfS[o]]) {
                found = true;
                newMoves[listOfS[i]] = newMoves[listOfS[o]];
                break;
            }
        }
        if (!found) {
            newMoves[listOfS[i]] = std::to_string(index);
            index++;
        }
    }
    return newMoves;
}

void ProcessMealyMode(const std::string& inputFileName, const std::string& outputFileName) {
    std::ifstream input(inputFileName);
    std::vector<std::string> listOfS = FillingList(input);

    std::vector<std::vector<Move>> originTable(listOfS.size());
    std::vector<std::vector<std::string>> tempTable(listOfS.size());
    std::vector<std::string> listOfX;
    std::string tempStr;
    while (getline(input, tempStr)) {
        tempStr += ";";
        listOfX.push_back(tempStr.substr(0, tempStr.find(";")));
        tempStr = tempStr.substr(tempStr.find(";") + 1, tempStr.size() - tempStr.find(";"));

        std::string tempMove;
        Move move;
        int columnNumber = 0;
        while (tempStr.find(";") != std::string::npos) {
            tempMove = tempStr.substr(0, tempStr.find(";"));
            tempStr = tempStr.substr(tempStr.find(";") + 1, tempStr.size() - tempStr.find(";"));

            move.y = tempMove.substr(tempMove.find('/') + 1);
            tempMove.resize(tempMove.find('/'));
            move.s = tempMove;

            originTable[columnNumber].push_back(move);
            tempTable[columnNumber].push_back(move.s);
            columnNumber++;
        }
    }
    input.close();

    std::unordered_map<std::string, std::string> moves1;
    int index1 = 1;
    for (int i = 0; i < listOfS.size(); i++) {
        bool found = false;
        for (int o = 0; o < i; o++) {
            bool equal = true;
            for (int p = 0; p < listOfX.size(); p++)
                if (originTable[i][p].y != originTable[o][p].y) {
                    equal = false;
                    break;
                }

            if (equal) {
                found = true;
                moves1[listOfS[i]] = moves1[listOfS[o]];
                break;
            }
        }
        if (!found) {
            moves1[listOfS[i]] = std::to_string(index1);
            index1++;
        }
    }
    std::unordered_map<std::string, std::string> moves2;

    bool changes = true;
    while (changes) {
        changes = false;

        for (int i = 0; i < originTable.size(); i++) {
            for (int o = 0; o < originTable[i].size(); o++) {
                tempTable[i][o] = moves1[originTable[i][o].s];
            }
        }

        moves2 = ReWriteMoves(moves1, listOfS, tempTable);

        if (moves1 != moves2) {
            changes = true;
            moves1 = moves2;
            moves2.clear();
        }
    }

    std::ofstream output(outputFileName);
    WritePositions(output, moves1);

    for (int i = 0; i < listOfX.size(); i++) {
        int index = 1;
        output << listOfX[i];
        int temp = 0;
        for (const auto& pair : moves1) {
            if (stoi(pair.second) >= index) {
                output << ";" << "F" << std::stoi(tempTable[temp][i]) - 1 << "/" << originTable[temp][i].y;
                index++;
            }
            temp++;
        }
        output << std::endl;
    }
    output.close();
}

void ProcessMooreMode(const std::string& inputFileName, const std::string& outputFileName) {
    std::ifstream input(inputFileName);

    std::vector<std::string> listOfQ = FillingList(input);
    std::vector<std::string> listOfS = FillingList(input);

    std::vector<std::vector<std::string>> originTable(listOfS.size());
    std::vector<std::string> listOfX;
    std::string tempStr;
    while (getline(input, tempStr)) {
        tempStr += ";";
        listOfX.push_back(tempStr.substr(0, tempStr.find(";")));
        tempStr = tempStr.substr(tempStr.find(";") + 1, tempStr.size() - tempStr.find(";"));

        int columnNumber = 0;
        while (tempStr.find(";") != std::string::npos) {
            originTable[columnNumber].push_back(tempStr.substr(0, tempStr.find(";")));
            tempStr = tempStr.substr(tempStr.find(";") + 1, tempStr.size() - tempStr.find(";"));
            columnNumber++;
        }
    }
    input.close();
    std::vector<std::vector<std::string>> tempTable = originTable;

    std::unordered_map<std::string, std::string> moves1;
    int index1 = 1;
    for (int i = 0; i < listOfS.size(); i++) {
        bool found = false;
        for (int o = 0; o < i; o++) {
            if (listOfQ[i] == listOfQ[o]) {
                found = true;
                moves1[listOfS[i]] = moves1[listOfS[o]];
                break;
            }
        }
        if (!found) {
            moves1[listOfS[i]] = std::to_string(index1);
            index1++;
        }
    }
    std::unordered_map<std::string, std::string> moves2;

    bool changes = true;
    while (changes) {
        changes = false;

        for (int i = 0; i < originTable.size(); i++) {
            for (int o = 0; o < originTable[i].size(); o++) {
                tempTable[i][o] = moves1[originTable[i][o]];
            }
        }

        moves2 = ReWriteMoves(moves1, listOfS, tempTable);

        if (moves1 != moves2) {
            changes = true;
            moves1 = moves2;
            moves2.clear();
        }
    }

    std::ofstream output(outputFileName);
    WriteListOfQ(output, moves1, listOfQ);
    WritePositions(output, moves1);

    for (int i = 0; i < listOfX.size(); i++) {
        int index = 1;
        output << listOfX[i];
        int temp = 0;
        for (const auto& pair : moves1) {
            if (stoi(pair.second) >= index) {
                output << ";" << "F" << std::stoi(tempTable[temp][i]) - 1;
                index++;
            }
            temp++;
        }
        output << std::endl;
    }
    output.close();
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cout << "Usage: <program> <mode(mealy/moore)> <input.csv> <output.csv>" << std::endl;
        return 1;
    }
    std::string mode = argv[1];
    std::string inputFileName = argv[2];
    std::string outputFileName = argv[3];

    if (mode == "mealy") {
        ProcessMealyMode(inputFileName, outputFileName);
    }
    else if (mode == "moore") {
        ProcessMooreMode(inputFileName, outputFileName);
    }
    else {
        std::cout << "Invalid mode. Choose 'mealy' or 'moore'." << std::endl;
        return 1;
    }

    std::cout << "Processing completed successfully." << std::endl;
    return 0;
}
