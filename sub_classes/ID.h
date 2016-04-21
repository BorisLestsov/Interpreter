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
    int value;
public:
    ID(lex_t lex_par, const string& name_par, int value = 0);
    ID(lex_t lex_par, const char* name_par, int value = 0);

    inline string get_name() const;
    inline lex_t get_type() const;
    inline int get_value() const;
};

class ID_table_t {
    vector<ID> table;
public:
    int append(const string& new_id, lex_t type_par, int value = 0);
    bool ispresent(const string name) const;
    void print_table() const;
};
