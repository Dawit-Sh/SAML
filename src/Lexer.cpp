/*
 * SAML 0.0.1
 * Author - Kidus W. (hello@kidusw.com)
 * License - GNU GPL v.3
 */

#include "Lexer.h"

void EXPORT SAML::Lexer::set_line(std::string& line)
{
    this->buffer = line;
    this->delimiter = " ";
    this->curr_pos = buffer.begin();
    this->m_unsortedTokens = this->split_string();
}

SAML::TokenClass SAML::Lexer::get_token_class(std::string& token)
{
    TokenClass _class = TK_NULL;

    if (token == "PRINT")
        _class = TK_PRINT;
    else if (token == "GET")
        _class = TK_GET;
    else if (token == "ECHO")
        _class = TK_ECHO;

    else if (token == "VAR")
        _class = TK_VAR;
    else if (token == "ARRAY")
        _class = TK_ARRAY;

    else if (token == "DEVIATE")
        _class = TK_DEVIATE;
    else if (token == "VARIANCE")
        _class = TK_VARIANCE;
    else if (token == "TOTAL")
        _class = TK_TOTAL;
    else if (token == "RANGE")
        _class = TK_RANGE;
    else if (token == "MEAN")
        _class = TK_MEAN;
    else if (token == "MEDIAN")
        _class = TK_MEDIAN;
    else if (token == "MODE")
        _class = TK_MODE;
    else if (token == "ADD")
        _class = TK_ADD;
    else if (token == "MUL")
        _class = TK_MUL;
    else if (token == "SUB")
        _class = TK_SUB;
    else if (token == "DIV")
        _class = TK_DIV;
    else if (token == "AT")
        _class = TK_AT;
    else if (token == "PUSH")
        _class = TK_PUSH;
    else if (is_numval(token))
        _class = TK_NUMVALUE;

    else
    {
        if (!is_legal_identifier(token))
        {
            _class = TK_ILLEGAL;
        }

        else {
            _class = TK_IDENTIFIER;
        }
    }

    return _class;
}

void EXPORT SAML::Lexer::lex()
{
    for (unsigned i = 0; i < this->m_unsortedTokens.size(); i++)
    {
        std::string _context = this->m_unsortedTokens[i];
        SAML::TokenClass tk_class = this->get_token_class(_context);

        Token _token;
        _token.m_tkclass = tk_class;
        _token.m_value = _context;

        this->m_sortedTokens.push_back(_token);
    }

    this->m_lexed = true;
}

EXPORT Tokenset SAML::Lexer::get_tokens()
{
    return this->m_sortedTokens;
}

void SAML::Lexer::skip_delim()
{
    while (curr_pos != buffer.end() && is_delim(*curr_pos))
        ++curr_pos;
}

bool SAML::Lexer::is_delim(char c)
{
    return (delimiter.find(c) != std::string::npos);
}

bool SAML::Lexer::is_numval(std::string& _token)
{
    bool return_val = true;

    for (unsigned i = 0; i < _token.length(); i++)
    {
        if (isalnum(_token[i]) && isdigit(_token[i]))
        {
            continue;
        }

        else {
            return_val = false;
            break;
        }
    }

    return return_val;
}

bool SAML::Lexer::is_legal_identifier(std::string& _token)
{
    bool return_val = true;

    for (unsigned i = 0; i < _token.length(); i++)
    {
        if (isalnum(_token[i])) {
            if (!is_numval(_token) && !isdigit(_token[0]))
            {
                continue;
            }

            else {
                return_val = false;
                break;
            }
        }

        else {
            return_val = false;
            break;
        }
    }

    if (is_numval(_token)) {
        return_val = false;
    }

    return return_val;
}

void SAML::Lexer::_clear()
{
    this->buffer.clear();
    this->delimiter.clear();
    this->m_lexed = false;
    this->m_sortedTokens.clear();
    this->m_unsortedTokens.clear();
    this->token.clear();
}

std::string SAML::Lexer::next()
{
    if (buffer.size() <= 0)
        return "";

    token.clear();

    this->skip_delim();

    while (curr_pos != buffer.end() && !is_delim(*curr_pos))
    {
        token += *curr_pos;
        ++curr_pos;
    }

    return token;
}

std::vector < std::string > SAML::Lexer::split_string()
{
    std::vector< std::string > tokens;
    std::string token;

    while ((token = this->next()) != "")
        tokens.push_back(token);

    return tokens;
}
