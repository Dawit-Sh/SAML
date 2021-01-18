/*
 * SAML 0.0.1
 * Author - Kidus W. (hello@kidusw.com)
 * License - GNU GPL v.3
 */

#include "Parser.h"

void EXPORT SAML::Parser::set_tokens(Lexer& lexer)
{
    if (!lexer.m_lexed)
    {
        //My favourite line of code.
        throw LexError();
    }

    this->m_tokens = lexer.get_tokens();
}

SAML::Tree EXPORT SAML::Parser::parse_tokens()
{
    Tree tree;
    std::vector < Token > _tokens = this->m_tokens;
    SAML::TokenClass tokens = _tokens[0].m_tkclass;
    std::vector < Token > _children;
    Action _action = ACT_NULL;

    /* Syntax Tree Structure:

                    *************
                    *Main Action*
                    *************
                         ||
                    ------------
                    |Child Node|
                    |          |
                    |----------|
                    |Child Node|
                    |          |
                    |----------|
                    |Child Node|
                    |          |
                    ------------
    */

    //Add child nodes
    for (unsigned i = 1; i < _tokens.size(); i++)
    {
        _children.push_back(_tokens[i]);
    }

    //Check and add main instruction
    if (tokens == TK_VAR)
        _action = ACT_ALLOC_VAR;
    else if (tokens == TK_ARRAY)
        _action = ACT_ALLOC_ARRAY;
    else if (tokens == TK_PRINT)
        _action = ACT_OUT;
    else if (tokens == TK_GET)
        _action = ACT_IN;
    else if (tokens == TK_ECHO)
        _action = ACT_ECHO;
    else if (tokens == TK_DEVIATE)
        _action = ACT_OP_DEVIATION;
    else if (tokens == TK_RANGE)
        _action = ACT_OP_RANGE;
    else if (tokens == TK_VARIANCE)
        _action = ACT_OP_VARIANCE;
    else if (tokens == TK_TOTAL)
        _action = ACT_OP_TOTAL;
    else if (tokens == TK_MEAN)
        _action = ACT_OP_MEAN;
    else if (tokens == TK_MEDIAN)
        _action = ACT_OP_MEDIAN;
    else if (tokens == TK_MODE)
        _action = ACT_OP_MODE;
    else if (tokens == TK_ADD)
        _action = ACT_OP_ADD;
    else if (tokens == TK_SUB)
        _action = ACT_OP_SUB;
    else if (tokens == TK_MUL)
        _action = ACT_OP_MUL;
    else if (tokens == TK_DIV)
        _action = ACT_OP_DIV;
    else if (tokens == TK_PUSH)
        _action = ACT_PUSH;
    else if (tokens == TK_IDENTIFIER || tokens == TK_ILLEGAL || tokens == TK_NULL || tokens == TK_NUMVALUE)
        _action = ACT_ERROR;

    tree.m_action = _action;
    tree.m_children = _children;

    return tree;
}
