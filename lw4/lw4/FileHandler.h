#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <string>
#include <vector>
#include <unordered_map>
#include <list>
#include <fstream>
#include "Automata.h"
#include "Utils.h"
#include "CollapsedState.h"

class FileHandler
{
public:
    static Automata ReadCSV(const std::string& filePath);

    static void WriteCSV
    (
        const std::string& filePath, 
        const std::vector<CollapsedState>& newProcessedStates, 
        const std::vector<NewTransition>& newTransitions, 
        const std::vector<std::string>& args
    );
};

#endif
