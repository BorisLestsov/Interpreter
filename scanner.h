//
// Created by Boris on 10.04.2016.
//

#pragma once

#include "Lex.h"
#include <vector>
#include "ID.h"
#include "Exception.h"

using namespace std;

enum macro_t {MACRO_NULL, MACRO_DEFINE, MACRO_IFDEF, MACRO_IFNDEF, MACRO_ELSE, MACRO_ENDIF, MACRO_UNDEF, DEFINE_FINISHED
};

class Scanner{
    enum state_t {H_ST, ID_ST, NUMB_ST, SIGN_ST, STR_ST, COM_ST, ALE_ST, DELIM_ST, EQ_ST, NEQ_ST, MACRO_ST, ADD_MACRO};
    state_t STATE;
    vector<Lex> lex_vec;
    FILE* f;

    char c;
    string buffer;

    //functions
    inline void gc();
    inline void clear_buffer();
    inline void addc();
    inline void addc(char my_c);
    int look(const string buf, const string table[]);
public:
    static const string WORD_NAMES[];
    static const string DEL_NAMES[];
    static const lex_t WORD_LEXEMS[];
    static const lex_t DEL_LEXEMS[];
    static const string MACRO_NAMES[];
    static ID_table_t ID_TABLE;


    static map<lex_t, string> lex_map;
    void construct_lex_map();

    Scanner(const char* input_f);
    Scanner();
    inline void add_lex(lex_t type_par, int val_par = 0);
    void print_vec() const;
    void start() throw(exception);
};

//TODO: proper exceptions - DONE
//TODO: check on permissible symbols - DONE
//TODO: defines - SYKA BLYAT
//TODO: why not to replace ID_ST with GET_ID_ST and PREV_STATE?
