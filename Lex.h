//
// Created by Boris on 10.04.2016.
//

#pragma once

#define NAME_LENGTH 32

#include <string>
#include <iomanip>
#include <ostream>
#include "lex_t.h"

using namespace std;

class Lex{
    lex_t type;
    int ptr;
public:
    Lex();
    Lex(lex_t lex_type, int ptr);
    Lex(const Lex &);
    ~Lex();

    friend ostream& operator<<(ostream& o, Lex& lex_par);

};

