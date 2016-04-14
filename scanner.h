//
// Created by Boris on 10.04.2016.
//

#pragma once

#include "Lex.h"
#include <vector>
#include <fstream>

#define MAXLEN

using namespace std;

class Scanner{
    vector<Lex> lex_vec;

    //а это вообще нормально?
    static vector<lex_t> WRD_VEC;
    static vector<lex_t> DLM_VEC;
    static vector<string> WRDS;
    static vector<string> DLMS;

    //должно ли это быть здесь?
    static vector<string> IDs;

    char c;
    FILE* f;

    string buffer;

    //функциии
    inline void add_lex(Lex& cur_lex);
    inline Lex get_last_lex();

    void print_vec();
    inline void clear_buffer();

public:
    Scanner(const char* input_f);

    //тут должно быть подобие get_lex()
};