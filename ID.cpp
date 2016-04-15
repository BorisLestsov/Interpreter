//
// Created by boris on 15.04.16.
//

#include "ID.h"

ID::ID(lex_t lex_par, const string& name_par): type(lex_par), name(name_par){}
ID::ID(lex_t lex_par, const char* name_par): type(lex_par), name(name_par){};

string ID::get_name() const{
    return name;
}

/*
string ID_table_t::get_name(int i) const{
    return table[i].get_name();
}*/

int ID_table_t::append(const string& new_id){
    int i = 0;
    vector<ID>::const_iterator ptr = table.cbegin();
    vector<ID>::const_iterator end  = table.cend();

    while(ptr != end){
        if(new_id == ptr->get_name())
            return i;
        ++i;
        ++ptr;
    }
    table.push_back(ID(LEX_ID, new_id));
    return i;
}

void ID_table_t::print_table(){
    vector<ID>::const_iterator ptr = table.cbegin();
    vector<ID>::const_iterator end  = table.cend();

    while(ptr != end){
        cout << ptr->get_name() << endl;
        ++ptr;
    }
}