//
// Created by Boris on 10.04.2016.
//

#include "parser.h"

Parser::Parser(const vector<Lex>& lex_vec_par, ID_table_t& ID_table_par, vector<ID_table_t>& STRUCT_vec_par):
        lex_vec(lex_vec_par),
        ID_table(ID_table_par),
        STRUCT_vec(STRUCT_vec_par)
{
    index = lex_vec.cbegin();
}

void Parser::get_lex() {
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
    get_lex();
    if(c_type == LEX_PROGRAM)
        DECLARATIONS();
    else throw Exception("Parser error: expected \"program\" but got lexem: ", Lex::lex_map[c_type]);
    get_lex();
    OPERATORS();
    /*if(c_type != LEX_FIN)
        throw Exception("Parser error: unexpected lexem: ", Lex::lex_map[c_type]);*/

}

void Parser::DECLARATIONS() {
    get_lex();
    if(c_type != LEX_BEGIN)
        throw Exception("Parser error: expected { but got lexem: ", Lex::lex_map[c_type]);
    //push { to stack?

    get_lex();
    while(c_type == LEX_STRUCT) {
        STRUCT_DECL();
        get_lex();
    }

    //gl();
    while(c_type == LEX_INT || c_type == LEX_BOOL || c_type == LEX_STRING){
        DECL();
        get_lex();
    }
}

void Parser::STRUCT_DECL() {
    int cur_struct_index;

    get_lex();
    if(c_type == LEX_ID) {
        cur_struct_index = ID_table[c_val].get_value();
    } else throw Exception("Parser error: expecded ID but got lexem: ", Lex::lex_map[c_type]);
    get_lex();
    if(c_type != LEX_BEGIN) throw Exception("Parser error: expected { but got lexem: ", Lex::lex_map[c_type]);
    get_lex();
    while(c_type == LEX_INT || c_type == LEX_BOOL || c_type == LEX_STRING){
        SWITCH_ID(STRUCT_vec[cur_struct_index]);
        get_lex();
    }
    if(c_type != LEX_END) throw Exception("Parser error: unexpected lexem: ", Lex::lex_map[c_type]);
    get_lex();
    while(c_type != LEX_SEMICOLON){
        if(c_type == LEX_ID){
            ID_table[c_val].set_type(LEX_STRUCT); //TODO: or LEX_STRUCT_T?
            //ID_table[c_val].set_val(); //TODO: WHAT IT IS SUPPOSED TO BE?
            ID_table[c_val].set_declared(true);
        } else throw Exception("Parser error: unexpected lexem: ", Lex::lex_map[c_type]);
        get_lex();
        if(c_type == LEX_COMMA)
            get_lex();
        if(c_type != LEX_ID && c_type != LEX_SEMICOLON)
            throw Exception("Parser error: unexpected lexem: ", Lex::lex_map[c_type]);
    }
}

void Parser::SWITCH_ID(ID_table_t& table) {
    switch (c_type) {
        case LEX_INT:
            while(c_type != LEX_SEMICOLON){
                get_lex();
                if(c_type == LEX_ID) {
                    if(table.multiple_declaration(table[c_val].get_name()))
                        throw Exception("Parser error: multiple declaration of : ", table[c_val].get_name());
                    table[c_val].set_declared(true);
                    table[c_val].set_type(LEX_INT);
                } else throw Exception("Parser error: expected ID but got lexem: ", Lex::lex_map[c_type]);
                make_tmp();
                get_lex();
                if(c_type == LEX_ASSIGN) {
                    get_lex();
                    if (c_type == LEX_NUM) {
                        table[tmp_val].set_assigned(true);
                        table[tmp_val].set_val(c_val);
                    } else throw Exception("Parser error: expected number but got lexem: ", Lex::lex_map[c_type]);
                    get_lex();
                } else
                    table[tmp_val].set_assigned(false);
                if(c_type == LEX_COMMA || c_type == LEX_SEMICOLON)
                    continue;
                else throw Exception("Parser error: unexpected lexem: ", Lex::lex_map[c_type]);
            }
            break;
        case LEX_BOOL:
            while(c_type != LEX_SEMICOLON){
                get_lex();
                if(c_type == LEX_ID) {
                    if(table.multiple_declaration(table[c_val].get_name()))
                        throw Exception("Parser error: multiple declaration of : ", table[c_val].get_name());
                    table[c_val].set_declared(true);
                    table[c_val].set_type(LEX_BOOL);
                } else throw Exception("Parser error: expected ID but got lexem: ", Lex::lex_map[c_type]);
                make_tmp();
                get_lex();
                if(c_type == LEX_ASSIGN) {
                    get_lex();
                    if (c_type == LEX_TRUE) {
                        table[tmp_val].set_assigned(true);
                        table[tmp_val].set_val(1);
                    } else if(c_type == LEX_FALSE){
                        table[tmp_val].set_assigned(true);
                        table[tmp_val].set_val(0);
                    } else
                        throw Exception("Parser error: expected bool constant but get lexem: ", Lex::lex_map[c_type]);
                    get_lex();
                } else
                    table[tmp_val].set_assigned(false);
                if(c_type == LEX_COMMA || c_type == LEX_SEMICOLON)
                    continue;
                else throw Exception("Parser error: unexpected lexem: ", Lex::lex_map[c_type]);
            }
            break;
        case LEX_STRING:
            while(c_type != LEX_SEMICOLON){
                get_lex();
                if(c_type == LEX_ID) {
                    if(table.multiple_declaration(table[c_val].get_name()))
                        throw Exception("Parser error: multiple declaration of : ", table[c_val].get_name());
                    table[c_val].set_declared(true);
                    table[c_val].set_type(LEX_STRING);
                } else throw Exception("Parser error: expected ID but got lexem: ", Lex::lex_map[c_type]);
                make_tmp();
                get_lex();
                if(c_type == LEX_ASSIGN){
                    get_lex();
                    if(c_type == LEX_STRC){
                        table[tmp_val].set_assigned(true);
                        table[tmp_val].set_val(c_val);
                    } else throw Exception("Parser error: expected string constant but got lexem: ", Lex::lex_map[c_type]);
                    get_lex();
                } else
                    table[tmp_val].set_assigned(false);
                if(c_type == LEX_COMMA || c_type == LEX_SEMICOLON)
                    continue;
                else throw Exception("Parser error: unexpected lexem: ", Lex::lex_map[c_type]);
            }
            break;
        default: throw Exception("Parser error: unexpected lexem: ", Lex::lex_map[c_type]);
    }
}

void Parser::DECL(){
    SWITCH_ID(ID_table);
}

void Parser::OPERATORS(){

}

void Parser::OP(){

}



void Parser::COMP_OP(){

}
