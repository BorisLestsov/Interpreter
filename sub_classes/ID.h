//
// Created by boris on 15.04.16.
//
#pragma once

#include "Lex.h"
#include <vector>
#include <string>
#include <iostream>

using namespace std;

class ID{
    lex_t type;
    string name;
    bool declared;
    bool assigned;
    int value;
public:
    ID(lex_t lex_par, const string& name_par, int value = 0);
    ID(lex_t lex_par, const char* name_par, int value = 0);

    //getters:
    string get_name() const;
    lex_t get_type() const;
    int get_value() const;
    bool get_declared() const;
    bool get_assigned() const;

    //setters:
    void set_name(const string& str);
    void set_type(lex_t type_par);
    void set_declared(bool bool_par);
    void set_assigned(bool bool_par);
    void set_val(int val_par);
};

class ID_table_t {
    vector<ID> table;
public:
    int append(const string& new_id, lex_t type_par, int value = 0);
    const ID* find(const string id_name) const;
    void erase(const string& name);
    ID& operator[] (int i);
    void print_table() const;
    bool multiple_declaration(int id) const;
};
