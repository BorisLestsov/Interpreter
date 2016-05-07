//
// Created by Boris on 10.04.2016.
//

#include "Lex.h"

Lex::Lex(): type(LEX_NULL) , value(0), add_value(0) {}
Lex::Lex(lex_t type_par, int value_par, int add_value_par):
    type(type_par),
    value(value_par), add_value(add_value_par){}
Lex::Lex(const Lex& lex_par): type(lex_par.type), value(lex_par.value), add_value(lex_par.add_value){};
Lex::~Lex(){};

lex_t Lex::get_type() const {
    return type;
}

int Lex::get_value() const {
    return value;
}

ostream& operator<<(ostream& o, const Lex& lex_par){
    o << setw(15) << Lex::lex_map[lex_par.type] << setw(15) << lex_par.value << setw(15) << lex_par.add_value;;
}

void operator++(lex_t& i ){
    i = (lex_t) ((int) i + 1 );
}

map<lex_t, string> Lex::lex_map;

void Lex::construct_lex_map(){
    lex_t i;
    string debug[] = {
            "LEX_NULL",
            "LEX_AND",
            "LEX_BEGIN",
            "LEX_BOOL",
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
            "LEX_TRUE",
            "LEX_VAR",
            "LEX_WHILE",
            "LEX_FOR",
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
            "LEX_STRUCT_T",
            "LEX_LABEL",
            "LEX_FIN",
            "LEX_NUM",
            "LEX_ID",
            "LEX_DOT",
            "RPN_GO",
            "RPN_FGO",
            "RPN_LABEL",
            "RPN_ADDRESS",
            "LEX_UMINUS",
            "LEX_UPLUS",
            "__END_TBL"
    };

    for(i = LEX_NULL; i != __END_TBL; ++i){
        lex_map.insert(make_pair(i , debug[i]));
    }
    lex_map.insert(make_pair(i, debug[i]));
}

int Lex::get_add_value() const {
    return add_value;
}
