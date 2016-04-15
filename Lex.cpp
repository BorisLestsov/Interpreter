//
// Created by Boris on 10.04.2016.
//

#include "Lex.h"

Lex::Lex(): type(LEX_NULL), table_ptr(WORD_NAMES) , value(0) {}
Lex::Lex(lex_t type_par, const string* table_ptr_par, int value_par):
    type(type_par), table_ptr(table_ptr_par), value(value_par){}
Lex::Lex(const Lex& lex_par): type(lex_par.type), table_ptr(lex_par.table_ptr), value(lex_par.value){};
Lex::~Lex(){};

ostream& operator<<(ostream& o, Lex& lex_par){
    
        o << setw(5) << lex_par.type << setw(10);
    if(type != LEX_ID)
        o << lex_par.table_ptr[lex_par.value];
    else 
        o << ID_TABLE.get_name(lex_par.value);
}


//tables
const string Lex::WORD_NAMES[] = {
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
        TBL_END
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
        TBL_END
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