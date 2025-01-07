#include "Lexer.h"
#include "Tokenizer.h"
#include "Logger.h"
#include <unordered_set>

namespace
{
    const std::unordered_set<char> IGNORED_CHARS = { ' ', '\t', '\n' };
    const std::unordered_set<char> SEPARATORS = { ' ', '\t', '\n', ';', ',', '{', '}', '(', ')', '[', ']', '=', '<', '>', '!', '/', '*', '+', '-', '"' };
}

Lexer::Lexer(std::istream& strm)
    : m_strm(strm)
{
    m_strm >> std::noskipws; 
}

Lexeme Lexer::GetLexeme()
{
    std::string lexeme;
    while (true)
    {
        try
        {
            lexeme = GetLexemeImpl();

            if (m_strm.eof() && lexeme.empty())
            {
                return { TokenType::END_OF_FILE, "", m_currentLine, m_currentPos };
            }

            if (!lexeme.empty())
            {
                break;
            }
        }
        catch (const std::exception& ex)
        {
            Logger::LogError("Exception in Lexer::GetLexeme: " + std::string(ex.what()));
            return { TokenType::ERROR, "", m_currentLine, m_currentPos };
        }
    }

    return { Tokenizer::ClassifyLexeme(lexeme), lexeme, m_currentLine, m_currentPos };
}

std::string Lexer::GetLexemeImpl()
{
    SkipIgnored();

    if (m_strm.eof())
    {
        return "";
    }

    char ch;
    std::string lexeme;
    while (!m_strm.eof() && GetNextChar(ch))
    {
        if (SEPARATORS.count(ch))
        {
            if (!lexeme.empty())
            {
                PutCharBack();
                break;
            }

            if (ch == '"')
            {
                return ProcessString();
            }

            return std::string(1, ch);
        }

        lexeme += ch;
    }

    return lexeme;
}

void Lexer::SkipIgnored()
{
    char ch;
    while (!m_strm.eof() && GetNextChar(ch))
    {
        if (!IGNORED_CHARS.count(ch))
        {
            PutCharBack();
            break;
        }
    }
}

std::string Lexer::ProcessString()
{
    std::string lexeme = "\"";
    char ch;
    while (!m_strm.eof() && GetNextChar(ch))
    {
        if (ch == '"')
        {
            lexeme += ch;
            return lexeme; 
        }
        lexeme += ch;
    }

    Logger::LogError("Unterminated string found.");
    return lexeme;
}

std::istream& Lexer::GetNextChar(char& ch)
{
    m_strm >> ch;
    ++m_currentPos;
    if (ch == '\n')
    {
        ++m_currentLine;
        m_currentPos = 0;
    }
    return m_strm;
}

void Lexer::PutCharBack()
{
    m_strm.unget();
    if (m_currentPos > 0)
    {
        --m_currentPos;
    }
}

void Lexer::UpdateCurrentLine(char ch)
{
    if (ch == '\n')
    {
        ++m_currentLine;
        m_currentPos = 0;
    }
}
