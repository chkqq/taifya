#include "Tokenizer.h"
#include <unordered_set>

namespace
{
    const std::unordered_set<std::string> KEYWORDS = {
        "void", "string", "double", "int", "bool",
        "while", "break", "continue", "if", "else", "return"
    };

    const std::unordered_set<std::string> MATH_OPERATIONS = {
        "+", "-", "*", "/", "="
    };

    const std::unordered_set<std::string> LOGICAL_OPERATORS = {
        "==", "&&", "||", "<", ">", "!"
    };

    const std::unordered_set<std::string> SEPARATORS = {
        "(", ")", "{", "}", "[", "]", ";", ","
    };

    bool IsNumber(const std::string& lexeme)
    {
        return std::regex_match(lexeme, std::regex(R"([-+]?\d+(\.\d+)?([eE][-+]?\d+)?)"));
    }

    bool IsIdentifier(const std::string& lexeme)
    {
        return std::regex_match(lexeme, std::regex(R"([a-zA-Z_][a-zA-Z0-9_]*)"));
    }
}

TokenType Tokenizer::ClassifyLexeme(const std::string& lexeme)
{
    if (KEYWORDS.count(lexeme)) return TokenType::KEYWORD;
    if (MATH_OPERATIONS.count(lexeme)) return TokenType::MATH_OPERATION;
    if (LOGICAL_OPERATORS.count(lexeme)) return TokenType::LOGICAL_OPERATOR;
    if (SEPARATORS.count(lexeme)) return TokenType::SEPARATOR;
    if (lexeme.front() == '"' && lexeme.back() == '"') return TokenType::STRING;
    if (lexeme == "true" || lexeme == "false") return TokenType::BOOLEAN;
    if (IsNumber(lexeme)) return TokenType::NUMBER;
    if (IsIdentifier(lexeme)) return TokenType::IDENTIFIER;
    return TokenType::ERROR;
}

std::string TokenTypeToString(TokenType type)
{
    switch (type)
    {
    case TokenType::KEYWORD:
        return "KEYWORD";
    case TokenType::IDENTIFIER:
        return "IDENTIFIER";
    case TokenType::SEPARATOR:
        return "SEPARATOR";
    case TokenType::MATH_OPERATION:
        return "MATH_OPERATION";
    case TokenType::LOGICAL_OPERATOR:
        return "LOGICAL_OPERATOR";
    case TokenType::STRING:
        return "STRING";
    case TokenType::NUMBER:
        return "NUMBER";
    case TokenType::BOOLEAN:
        return "BOOLEAN";
    case TokenType::END_OF_FILE:
        return "END_OF_FILE";
    case TokenType::ERROR:
    default:
        return "ERROR";
    }
}