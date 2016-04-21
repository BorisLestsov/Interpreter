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