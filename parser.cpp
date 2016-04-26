//
// Created by Boris on 10.04.2016.
//

#include "parser.h"

Parser::Parser(const vector<Lex>& lex_vec_par, ID_table_t& ID_table_par):
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

void Parser::make_tmp() {
    tmp_type = c_type;
    tmp_val = c_val;
}


void Parser::start() {
    PROGRAM();
}

void Parser::PROGRAM(){
    gl();
    if(c_type == LEX_PROGRAM)
        DECLARATIONS();
    else throw Exception("Parser error: expected \"program\" but got lexem: ", Lex::lex_map[c_type]);
    gl();
    OPERATORS();
    /*if(c_type != LEX_FIN)
        throw Exception("Parser error: unexpected lexem: ", Lex::lex_map[c_type]);*/

}

void Parser::DECLARATIONS() {
    gl();
    if(c_type != LEX_BEGIN)
        throw Exception("Parser error: expected { but got lexem: ", Lex::lex_map[c_type]);
    //push { to stack?

    gl();
    while(c_type == LEX_STRUCT) {
        STRUCT_DECL();
        gl();
    }

    //gl();
    while(c_type == LEX_INT || c_type == LEX_BOOL || c_type == LEX_STRING){
        DECL();
        gl();
    }
}

void Parser::STRUCT_DECL() {
    gl();
    if(c_type != LEX_ID) throw Exception("Parser error: expecded ID but got lexem: ", Lex::lex_map[c_type]);
    gl();
    if(c_type != LEX_BEGIN) throw Exception("Parser error: expected { but got lexem: ", Lex::lex_map[c_type]);
    gl();
    while(c_type == LEX_INT || c_type == LEX_BOOL || c_type == LEX_STRING){
        gl();
        while(c_type != LEX_SEMICOLON){
            if(c_type == LEX_ID) {
                ID_table[c_val].set_declared(true);
                ID_table[c_val].set_type(LEX_INT);
            } else throw Exception("Parser error: expected ID but got lexem: ", Lex::lex_map[c_type]);
            make_tmp();
            gl();
            if(c_type == LEX_ASSIGN) {
                gl();
                if (c_type == LEX_NUM) {
                    ID_table[tmp_val].set_assigned(true);
                    ID_table[tmp_val].set_val(c_val);
                } else throw Exception("Parser error: expected number but got lexem: ", Lex::lex_map[c_type]);
                gl();
            } else
                ID_table[tmp_val].set_assigned(false);
            if(c_type == LEX_COMMA || c_type == LEX_SEMICOLON)
                continue;
            else throw Exception("Parser error: unexpected lexem: ", Lex::lex_map[c_type]);
        }
    }
}


void Parser::DECL(){
    switch (c_type) {
        case LEX_INT:
            while(c_type != LEX_SEMICOLON){
                gl();
                if(c_type == LEX_ID) {
                    ID_table[c_val].set_declared(true);
                    ID_table[c_val].set_type(LEX_INT);
                } else throw Exception("Parser error: expected ID but got lexem: ", Lex::lex_map[c_type]);
                make_tmp();
                gl();
                if(c_type == LEX_ASSIGN) {
                    gl();
                    if (c_type == LEX_NUM) {
                        ID_table[tmp_val].set_assigned(true);
                        ID_table[tmp_val].set_val(c_val);
                    } else throw Exception("Parser error: expected number but got lexem: ", Lex::lex_map[c_type]);
                    gl();
                } else
                    ID_table[tmp_val].set_assigned(false);
                if(c_type == LEX_COMMA || c_type == LEX_SEMICOLON)
                    continue;
                else throw Exception("Parser error: unexpected lexem: ", Lex::lex_map[c_type]);
            }
            break;
        case LEX_BOOL:
            while(c_type != LEX_SEMICOLON){
                gl();
                if(c_type == LEX_ID) {
                    ID_table[c_val].set_declared(true);
                    ID_table[c_val].set_type(LEX_BOOL);
                } else throw Exception("Parser error: expected ID but got lexem: ", Lex::lex_map[c_type]);
                make_tmp();
                gl();
                if(c_type == LEX_ASSIGN) {
                    gl();
                    if (c_type == LEX_TRUE || c_type == LEX_FALSE) {
                        ID_table[tmp_val].set_assigned(true);
                        ID_table[tmp_val].set_val(c_val);
                    } else
                        throw Exception("Parser error: expected bool constant but get lexem: ", Lex::lex_map[c_type]);
                    gl();
                } else
                    ID_table[tmp_val].set_assigned(false);
                 if(c_type == LEX_COMMA || c_type == LEX_SEMICOLON)
                    continue;
                else throw Exception("Parser error: unexpected lexem: ", Lex::lex_map[c_type]);
            }
            break;
        case LEX_STRING:
            while(c_type != LEX_SEMICOLON){
                gl();
                if(c_type == LEX_ID) {
                    ID_table[c_val].set_declared(true);
                    ID_table[c_val].set_type(LEX_STRING);
                } else throw Exception("Parser error: expected ID but got lexem: ", Lex::lex_map[c_type]);
                make_tmp();
                gl();
                if(c_type == LEX_ASSIGN){
                    gl();
                    if(c_type == LEX_STRC){
                        ID_table[tmp_val].set_assigned(true);
                        ID_table[tmp_val].set_val(c_val);
                    } else throw Exception("Parser error: expected string constant but got lexem: ", Lex::lex_map[c_type]);
                    gl();
                } else
                    ID_table[tmp_val].set_assigned(false);
                if(c_type == LEX_COMMA || c_type == LEX_SEMICOLON)
                    continue;
                else throw Exception("Parser error: unexpected lexem: ", Lex::lex_map[c_type]);
            }
            break;
        default: throw Exception("Parser error: unexpected lexem: ", Lex::lex_map[c_type]);
    }
}

void Parser::OPERATORS(){

}

void Parser::OP(){

}



void Parser::COMP_OP(){

}
