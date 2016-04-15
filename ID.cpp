//
// Created by boris on 15.04.16.
//

#include "ID.h"

ID::ID(lex_t lex_par, string& name_par): type(lex_par), name(name_par){}
ID::ID(lex_t lex_par, const char* name_par): type(lex_par), name(name_par){};

string ID::get_name() const{
    return name;
}

void ID_table_t::add(ID id_par){

}

string ID_table_t::get_name(int i) const{
    return table[i].get_name();
}