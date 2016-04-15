//
// Created by boris on 15.04.16.
//
#pragma once

#include "Lex.h"
#include <vector>
#include <string>

using namespace std;

class ID{
    lex_t type;
    string name;
public:
    ID(lex_t lex_par, string& name_par);
    ID(lex_t lex_par, const char* name_par);

    inline string get_name() const;
};

class ID_table_t {
    vector<ID> table;
public:
    inline string get_name(int i) const;
    void add(ID id_par);
};
