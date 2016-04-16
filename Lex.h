//
// Created by Boris on 10.04.2016.
//

#pragma once

#define NAME_LENGTH 32
#define TBL_END "__t_end"

#include <string>
#include <iomanip>
#include <ostream>
#include "lex_t.h"
#include <map>

void operator++(lex_t& i);

using namespace std;

class Lex{
    lex_t type;
    int value;

public:
    Lex();
    Lex(lex_t type_par, int value_par = 0);
    Lex(const Lex &);
    ~Lex();
    lex_t get_type() const;
    int get_value() const;

    friend ostream& operator<<(ostream& o, const Lex& lex_par);
};