//
// Created by boris on 05.05.16.
//

#pragma once

#include <ID.h>
#include "RPN.h"

extern int DEBUG_MODE;

class Virtual_Machine {
    RPN& prog;
    vector<Lex>::const_iterator cmd_ptr;
    Lex c_cmd;
    vector<ID_table_t>& ID_tables_vec;
    Lex c_lex;
    Lex arg1;
    Lex arg2;
    string tmp_str;
    int tmp1, tmp2;

    inline void get_cmd();
    inline bool prog_finished() const;
public:
    Virtual_Machine(RPN& prog_par, vector<ID_table_t>& ID_tables_vec_par);

    int start();
};

//TODO: fix the problem when STRC and ID have same names