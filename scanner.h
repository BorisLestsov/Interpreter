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

    //functions
    inline void gc();
    inline void clear_buffer();
    inline void addc();
    int look(const string buf, const string table[]);
public:
    static const string WORD_NAMES[];
    static const string DEL_NAMES[];
    static const lex_t WORD_LEXEMS[];
    static const lex_t DEL_LEXEMS[];
    static ID_table_t ID_TABLE;

    Scanner(const char* input_f);
    Scanner();
    void print_vec();
    void start();
};
