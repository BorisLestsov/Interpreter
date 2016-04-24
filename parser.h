//
// Created by Boris on 10.04.2016.
//

#pragma once

#include "ID.h"
#include "Lex.h"
#include "Exception.h"
#include <vector>
#include "ID.h"

using namespace std;

extern map<lex_t, string> lex_map;

class Parser{
    const vector<Lex>& lex_vec;
    const ID_table_t& ID_table;
    vector<Lex>::const_iterator index;
    Lex c_lex;
    lex_t c_type;
    int c_val;

    //procedures:
    void PROGRAM();
    void DESCRIPTIONS();
    void OPERATORS();
    void DESC();
    void TYPE();
    void VAR();
    //void CONST();
    void OP();
    void COMP_OP();

    //semantics checks:

    void gl();
public:
    Parser(const vector<Lex>& lex_vec_par, const ID_table_t& ID_table_par);
    void start();
};

//TODO: Build Parser:
//1) just procedures without semantic checks
//2) with semantic checks
//3) with poliz