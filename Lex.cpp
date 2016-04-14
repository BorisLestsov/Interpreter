//
// Created by Boris on 10.04.2016.
//

#include "Lex.h"

Lex::Lex(): type(LEX_NULL), ptr(0) {}
Lex::Lex(lex_t type_par, int ptr_par): type(type_par), ptr(ptr_par) {}
Lex::Lex(const Lex& lex_par): type(lex_par.type), ptr(lex_par.ptr){};
Lex::~Lex(){};

ostream& operator<<(ostream& o, Lex& lex_par){
    o << setw(5) << lex_par.type << setw(5) << lex_par.ptr;
}
