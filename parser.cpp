//
// Created by Boris on 10.04.2016.
//

#include "parser.h"

Parser::Parser(const vector<Lex>& lex_vec_par, const ID_table_t& ID_table_par):
        lex_vec(lex_vec_par),
        ID_table(ID_table_par)
{
    index = lex_vec.cbegin();
}

void Parser::gl() {
    c_lex = *index;
    c_type = c_lex.get_type();
    c_val = c_lex.get_value();
    ++index;
}

void Parser::start() {
    gl();
    PROGRAM();
}

void Parser::PROGRAM(){
    if(c_type == LEX_PROGRAM)
        DESCRIPTIONS();
    else throw Exception("Parser error: unexpected lexem: ", Lex::lex_map[c_type]);
}

void Parser::DESCRIPTIONS(){

}

void Parser::OPERATORS(){

}

void Parser::DESC(){

}

void Parser::TYPE(){

}

void Parser::VAR(){

}

void Parser::OP(){

}


void Parser::COMP_OP(){

}
