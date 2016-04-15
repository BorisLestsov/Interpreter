//
// Created by Boris on 10.04.2016.
//

#pragma once

#include "Lex.h"
#include <vector>
#include <fstream>
#include "ID.h"

using namespace std;

class Scanner{
    enum state_t {H_ST, ID_ST, NUMB_ST, COM_ST, ALE_ST, DELIM_ST, NEQ_ST };
    state_t STATE;
    vector<Lex> lex_vec;
    FILE* f;

    char c;
    string buffer;

    //здесь ли это должно быть?

    //функциии
    inline void add_lex(Lex& cur_lex);
    inline void gc();
    inline void clear_buffer();
    inline void addc();
    int look(const string buf, const string table[]);
public:
    static ID_table_t ID_TABLE;

    Scanner(const char* input_f);
    Scanner();
    void print_vec();
    void start();
};
