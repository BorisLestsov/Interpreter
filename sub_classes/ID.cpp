//
// Created by boris on 15.04.16.
//

#include "ID.h"
#include "Exception.h"

ID::ID(lex_t lex_par, const string& name_par, int val_par): type(lex_par), name(name_par), value(val_par){}
ID::ID(lex_t lex_par, const char* name_par, int val_par): type(lex_par), name(name_par), value(val_par){};

string ID::get_name() const{
    return name;
}

lex_t ID::get_type() const{
    return type;
}

int ID::get_value() const{
    return value;
}

/*
string ID_table_t::get_name(int i) const{
    return table[i].get_name();
}*/

int ID_table_t::append(const string &new_id, lex_t type_par, int value) {
    int i = 0;
    vector<ID>::const_iterator ptr = table.cbegin();
    vector<ID>::const_iterator end  = table.cend();

    while(ptr != end){
        if(new_id == ptr->get_name())
            return i;
        ++i;
        ++ptr;
    }
    table.push_back(ID(type_par, new_id, value));
    return i;
}

const ID* ID_table_t::find(const string id_name) const{
    vector<ID>::const_iterator ptr = table.cbegin();
    vector<ID>::const_iterator end  = table.cend();

    while(ptr != end){
        if(id_name == ptr->get_name())
            return &(*ptr);
        ++ptr;
    }
    return NULL;
}

void ID_table_t::erase(const string& id_name){
    vector<ID>::iterator ptr = table.begin();
    vector<ID>::iterator end  = table.end();

    while(ptr != end){
        if(id_name == ptr->get_name()){
            table.erase(ptr);
            return;
        }
        ++ptr;
    }
    throw Exception("Scanner error: identifier not found: ", id_name);
}

void ID_table_t::print_table() const{
    vector<ID>::const_iterator ptr = table.cbegin();
    vector<ID>::const_iterator end  = table.cend();

    cout << "------FOUND IDENTIFIERS:------" << endl;
    while(ptr != end){
        cout << setw(15) << Lex::lex_map[ptr->get_type()] << setw(15) <<
                ptr->get_name() << setw(15) << ptr->get_value() << endl;
        ++ptr;
    }
}