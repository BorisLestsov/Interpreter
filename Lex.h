//
// Created by Boris on 10.04.2016.
//

#pragma once

#define NAME_LENGTH 32

#include <string>
#include <iomanip>
#include <ostream>

using namespace std;

typedef enum{
    LEX_0,
    lex1,
    lex2
} lex_t;



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


