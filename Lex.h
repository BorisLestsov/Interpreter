//
// Created by Boris on 10.04.2016.
//

#ifndef INTERPRETER_LEX_H
#define INTERPRETER_LEX_H

#define NAME_LENGTH 32

#include <string>

typedef enum{
    lex1,
    lex2
} lex_t;

class Lex{
    lex_t type;
    string name[NAME_LENGTH];
public:
    Lex(lex_t lex_type, char* name);
    Lex(Lex &);
    ~Lex();

};



#endif //INTERPRETER_LEX_H
