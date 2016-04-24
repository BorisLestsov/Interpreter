//
// Created by Boris on 10.04.2016.
//

#include "Lex.h"

Lex::Lex(): type(LEX_NULL) , value(0) {}
Lex::Lex(lex_t type_par, int value_par):
    type(type_par), value(value_par){}
Lex::Lex(const Lex& lex_par): type(lex_par.type), value(lex_par.value){};
Lex::~Lex(){};

lex_t Lex::get_type() const {
    return type;
}

int Lex::get_value() const {
    return value;
}

ostream& operator<<(ostream& o, const Lex& lex_par){
    o << setw(5) << lex_par.type << setw(10)  << lex_par.value;
}

void operator++(lex_t& i ){
    i = (lex_t) ((int) i + 1 );
}


string debug[] = {
        "LEX_NULL",
        "LEX_AND",
        "LEX_BEGIN",
        "LEX_BOOL",
        "LEX_DO",
        "LEX_ELSE",
        "LEX_END",
        "LEX_IF",
        "LEX_FALSE",
        "LEX_INT",
        "LEX_STRING",
        "LEX_NOT",
        "LEX_OR",
        "LEX_PROGRAM",
        "LEX_READ",
        "LEX_THEN",
        "LEX_TRUE",
        "LEX_VAR",
        "LEX_WHILE",
        "LEX_WRITE",
        "LEX_SEMICOLON",
        "LEX_COMMA",
        "LEX_COLON",
        "LEX_EQ",
        "LEX_LPAREN",
        "LEX_RPAREN",
        "LEX_ASSIGN",
        "LEX_LSS",
        "LEX_GTR",
        "LEX_PLUS",
        "LEX_MINUS",
        "LEX_TIMES",
        "LEX_SLASH",
        "LEX_LEQ",
        "LEX_NEQ",
        "LEX_GEQ",
        "LEX_STRC",
        "LEX_STRUCT",
        "LEX_GOTO",
        "LEX_BREAK",
        "LEX_CONTINUE",
        "LEX_MACRO_NAME",
        "LEX_FIN",
        "LEX_NUM",
        "LEX_ID",
};

map<lex_t, string> Lex::lex_map;

void Lex::construct_lex_map(){
    lex_t i;

    for(i = LEX_NULL; i != LEX_ID; ++i){
        lex_map.insert(make_pair(i , debug[i]));
    }
    lex_map.insert(make_pair(i, debug[i]));
}
