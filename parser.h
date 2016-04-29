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
#include "RPN.h"
#include "ID.h"

using namespace std;

class Parser{
    const vector<Lex>& lex_vec;
    ID_table_t& ID_table;
    vector<Lex>::const_iterator index;
    vector<ID_table_t>& STRUCT_vec;
    stack<lex_t> lex_stack;
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
    void COMPLEX_OP();
    void EXPRESSION();
    void HIGH();
    void LOW();
    void F();


    void SWITCH_ID(ID_table_t& table);

    //semantics checks:
    void eq_bool();
    void check_id();
    void check_op();
    void check_not();
    void check_assign();

    void get_lex();
    void make_tmp();
public:
    RPN prog;

    Parser(const vector<Lex>& lex_vec_par, ID_table_t& ID_table_par, vector<ID_table_t>& STRUCT_vec_par);
    void start();
};

//TODO: how to show wrong string of user's program?
//TODO: Implement "for", "break", "goto" and also structure fields access: s.i = 5;