//
// Created by Boris on 10.04.2016.
//

#ifndef INTERPRETER_SCANNER_H
#define INTERPRETER_SCANNER_H

#include "Lex.h"
#include <vector>

using namespace std;

class Scanner{
    vector<Lex> lex_vec;
public:
    Scanner(): lex_vec(0){}

    void add_lex(Lex & cur_lex){
        lex_vec.push_back(cur_lex);
    }
    Lex get_last_lex(){
        return lex_vec.back();
    }


};


#endif //INTERPRETER_SCANNER_H
