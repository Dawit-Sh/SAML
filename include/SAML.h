/*
 * SAML 0.0.1
 * Written by Kidus W. (hello@kidusw.com)
 */

#ifndef SAML_H
#define SAML_H

#define EXPORT __declspec(dllexport)
#define IMPORT __declspec(dllimport)

#define WIN32_LEAN_AND_MEAN

#define MAX_ARRAY_SIZE 512

#include <iostream>
#include <string>
#include <vector>
#include <windows.h>

namespace SAML {
    void EXPORT API_Test();

    enum TokenClass
    {
        TK_PRINT,
        TK_GET,
        TK_ECHO,
        TK_AT,
        TK_VAR,
        TK_ARRAY,
        TK_INTO,
        TK_DEVIATE,
        TK_RANGE,
        TK_TOTAL,
        TK_MEAN,
        TK_MODE,
        TK_MEDIAN,
        TK_VARIANCE,
        TK_MUL,
        TK_SUB,
        TK_DIV,
        TK_ADD,
        TK_PUSH,
        TK_IDENTIFIER,
        TK_ILLEGAL,
        TK_NUMVALUE,
        TK_NULL
    };

    enum Action {
        ACT_ALLOC_VAR,
        ACT_ALLOC_ARRAY,
        ACT_OUT,
        ACT_IN,
        ACT_ECHO,
        ACT_PUSH,
        ACT_REALLOC,
        ACT_OP_RANGE,
        ACT_OP_DEVIATION,
        ACT_OP_MEAN,
        ACT_OP_MEDIAN,
        ACT_OP_MODE,
        ACT_OP_TOTAL,
        ACT_OP_VARIANCE,
        ACT_OP_ADD,
        ACT_OP_SUB,
        ACT_OP_MUL,
        ACT_OP_DIV,
        ACT_ERROR,
        ACT_NULL
    };

    class Token {
    public:
        std::string m_value;
        TokenClass m_tkclass;
    };

    class Tree {
    public:
        Action m_action;
        std::vector < Token > m_children;
    };

    class Lexer {
    public:
        void EXPORT set_line(std::string& line);
        void EXPORT lex();
        EXPORT std::vector < SAML::Token > get_tokens();
        void EXPORT _clear();

        bool m_lexed;

    private:
        void skip_delim();
        bool is_delim(char c);
        bool is_numval(std::string& token);
        bool is_legal_identifier(std::string& token);
        std::string next();
        std::vector < std::string > split_string();
        TokenClass get_token_class(std::string& token);

        std::vector < std::string > m_unsortedTokens;
        std::vector < Token > m_sortedTokens;

        std::string buffer;
        std::string token;
        std::string delimiter;
        std::string::const_iterator curr_pos;
    };

    class Parser {
    public:
        void EXPORT set_tokens(Lexer& lexer);
        Tree EXPORT parse_tokens();

    private:
        std::vector < Token > m_tokens;
    };

    class Variable {
    public:
        std::string m_identifier;
        double m_value;
        bool comp(Variable& _var);
    };

    class Array {
    public:
        std::string m_identifier;
        std::vector < double > m_value;
        bool comp(Array& _arr);
    };

    class Stack {
    public:
        void add(Variable& var);
        void add(Array& arr);
        Variable retrieve_var(std::string& identifier);
        Array retrieve_array(std::string& identifier);
        bool var_exists(Variable& variable);
        bool arr_exists(Array& array);

    private:
        std::vector < Variable > m_varstack;
        std::vector < Array > m_arrstack;
    };

    bool EXPORT evaluate_ast(Tree& tree);

    //Algorithms:
    double deviate_list(std::vector < double >& values);
    double find_mean(std::vector < double >& values);
    double find_mode(std::vector < double >& values);
    double find_median(std::vector < double >& values);
    double find_range(std::vector < double >& values);
    double find_variance(std::vector < double >& values);
    double find_total(std::vector < double >& values);
}

#endif // SAML_H
