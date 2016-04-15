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
#include "ID.h" 


using namespace std;

class Lex{
    lex_t type;
    const string* table_ptr;
    int value;

public:
    static const string WORD_NAMES[];
    static const string DEL_NAMES[];
    static const lex_t WORD_LEXEMS[];
    static const lex_t DEL_LEXEMS[];

    Lex();
    Lex(lex_t type_par, const string* table_ptr_par, int value_par);
    Lex(const Lex &);
    ~Lex();

    friend ostream& operator<<(ostream& o, Lex& lex_par);
};

extern ID_table_t ID_TABLE;