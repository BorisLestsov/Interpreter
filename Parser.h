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
    vector<Lex>::const_iterator index;

    stack<int> break_stack;

    stack<Lex> lex_stack;
    Lex c_lex;
    lex_t c_type;
    int c_val;
    int c_add_val;
    lex_t tmp_type;
    int tmp_val;
    int tmp_add_val;

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
    void ULTRA_ULTRA_HIGH();
    void ULTRA_HIGH();
    void HIGH();
    void MID();
    void LOW();
    void F();


    void SWITCH_ID(ID_table_t& table);

    //semantics checks:
    void eq_bool();
    void check_id();
    void check_op();
    void check_not();
    void check_assign();
    void check_left_op();

    inline void get_lex();
    inline void unget_lex();
    inline void make_tmp();
    void struct_init();
public:
    vector<ID_table_t>& ID_tables_vec;
    RPN prog;

    Parser(const vector<Lex>& lex_vec_par, vector<ID_table_t>& STRUCT_vec_par);
    void start();
};

//TODO: how to show wrong string in user's program?
