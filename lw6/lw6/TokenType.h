#pragma once
#include <map>

enum class TokenType
{
    IDENTIFIER,
    KEYWORD,
    SEPARATOR,
    MATH_OPERATION,
    LOGICAL_OPERATOR,
    STRING,
    NUMBER,
    BOOLEAN,
    END_OF_FILE,
    ERROR,
};
