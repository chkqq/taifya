#include <iostream>
#include "FileHandler.h"
#include "Lexer.h"
#include "Logger.h"

int main()
{
    try
    {
        FileHandler fileHandler("input1.txt", "output1.txt");
        Lexer lexer(fileHandler.GetInputStream());

        while (true)
        {
            Lexeme lexeme = lexer.GetLexeme();
            if (lexeme.type == TokenType::END_OF_FILE)
                break;

            fileHandler.GetOutputStream() << "[" << lexeme.lineNum << ":" << lexeme.linePos << "] "
                << lexeme.lexeme << " <- " << TokenTypeToString(lexeme.type) << std::endl;
        }

        Logger::LogInfo("Processing complete.");
    }
    catch (const std::exception& ex)
    {
        Logger::LogError(ex.what());
    }

    return 0;
}
