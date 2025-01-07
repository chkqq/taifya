#include "FileHandler.h"


std::list<std::string> FileHandler::GetUniqueArgs(const std::vector<Transition>& transitions)
{
    std::list<std::string> args;
    for (const auto& trans : transitions)
        args.push_back(trans.arg);
    args.sort();
    args.unique();
    return args;
}

void FileHandler::WriteCSV(const std::string& filename, const std::unordered_map<std::string, std::string>& states, const std::vector<Transition>& transitions)
{
    std::ofstream output(filename);
    std::list<std::string> tempList;

    for (const auto& s : states)
        tempList.push_back(s.second);
    tempList.sort();
    output << "F" << std::endl;

    output << ";";
    for (const auto& s : tempList)
        output << s << ";";

    for (const auto& arg : GetUniqueArgs(transitions))
    {
        output << arg << ";";
        for (const auto& from : tempList)
        {
            std::string temp;
            for (const auto& trans : transitions)
            {
                if (trans.from == from && trans.arg == arg)
                    temp += trans.to + ",";
            }
            if (!temp.empty())
                temp = temp.substr(0, temp.size() - 1);
            output << temp << ";";
        }
        output << std::endl;
    }
}