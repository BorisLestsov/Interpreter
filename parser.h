//
// Created by Boris on 10.04.2016.
//

#pragma once

#include "ID.h"
#include "Lex.h"
#include "Exception.h"
#include <vector>
#include <stack>
#include <map>
#include "ID.h"

using namespace std;

class Parser{
    const vector<Lex>& lex_vec;
    ID_table_t& ID_table;
    vector<Lex>::const_iterator index;
    Lex c_lex;
    lex_t c_type;
    int c_val;
    lex_t tmp_type;
    int tmp_val;

    //procedures:
    void PROGRAM();
    void DECLARATIONS();
    void STRUCT_DECL();
    void OPERATORS();
    void DECL();
    //void CONST();
    void OP();
    void COMP_OP();


    //semantics checks:


    void gl();
    void make_tmp();
public:
    Parser(const vector<Lex>& lex_vec_par, ID_table_t& ID_table_par);
    void start();
};

//TODO: how to store structures?
//TODO: how to show wrong string of user's program?
//TODO: Build Parser:
//1) just procedures without semantic checks
//2) with semantic checks
//3) with poliz