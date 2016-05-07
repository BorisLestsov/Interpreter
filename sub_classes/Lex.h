//
// Created by Boris on 10.04.2016.
//

#pragma once

#define NAME_LENGTH 32
#define TBL_END "__t_end"

#include <string>
#include <iomanip>
#include <iostream>
#include "lex_t.h"
#include <map>

void operator++(lex_t& i);

using namespace std;

class Lex{
    lex_t type;
    int value;
    int add_value;

public:
    Lex();
    Lex(lex_t type_par, int value_par = 0, int add_value_par = 0);
    Lex(const Lex &);
    ~Lex();
    lex_t get_type() const;
    int get_value() const;
    int get_add_value() const;
    void set_add_value(int val);


    friend ostream& operator<<(ostream& o, const Lex& lex_par);

    static map<lex_t, string> lex_map;
    static void construct_lex_map();
};