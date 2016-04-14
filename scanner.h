//
// Created by Boris on 10.04.2016.
//

#pragma once

#include "Lex.h"
#include <vector>
#include <fstream>

using namespace std;

class Scanner{
    enum state_t {};
    vector<Lex> lex_vec;
    state_t cur_state;

    char c;
    FILE* f;

    static const string WORD_NAMES[];
    static const string DEL_NAMES[];
    static const lex_t WORD_LEXEMS[];
    static const lex_t DEL_LEXEMS[];
    static vector<string> IDs;  //здесь ли это должно быть?

    string buffer;

    //функциии
    inline void add_lex(Lex& cur_lex);
    inline void gc();
    inline void clear_buffer();
    inline void addc();
    int look(string buf, string table[]);
public:
    Scanner(const char* input_f);
    void print_vec();

    //тут должно быть подобие get_lex()
};

    //tables
const string WORD_NAMES[] = {
    "",
    "and",
    "begin",
    "bool",
    "do",
    "else",
    "end",
    "if",
    "false",
    "int",
    "not",
    "or",
    "program",
    "read",
    "then",
    "true",
    "var",
    "while",
    "write",
    NULL
};

const string DEL_NAMES[] = {
    "",
    "@",
    ";",
    ",",
    ":",
    ":=",
    "(",
    ")",
    "=",
    "<",
    ">",
    "+",
    "-",
    "*",
    "/",
    "<=",
    "!=",
    ">=",
    NULL
};

const lex_t WORD_LEXEMS[] = {
    LEX_NULL,
    LEX_AND,
    LEX_BEGIN,
    LEX_BOOL,
    LEX_DO,
    LEX_ELSE,
    LEX_END,
    LEX_IF,
    LEX_FALSE,
    LEX_INT,
    LEX_NOT,
    LEX_OR,
    LEX_PROGRAM,
    LEX_READ,
    LEX_THEN,
    LEX_TRUE,
    LEX_VAR,
    LEX_WHILE,
    LEX_WRITE,
    LEX_NULL
};

const lex_t DEL_LEXEMS[] = {
    LEX_NULL,
    LEX_FIN,
    LEX_SEMICOLON,
    LEX_COMMA,
    LEX_COLON,
    LEX_ASSIGN,
    LEX_LPAREN,
    LEX_RPAREN,
    LEX_EQ,
    LEX_LSS,
    LEX_GTR,
    LEX_PLUS,
    LEX_MINUS,
    LEX_TIMES,
    LEX_SLASH,
    LEX_LEQ,
    LEX_NEQ,
    LEX_GEQ,
    LEX_NULL
};