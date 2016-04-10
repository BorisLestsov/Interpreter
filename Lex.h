//
// Created by Boris on 10.04.2016.
//

#ifndef INTERPRETER_LEX_H
#define INTERPRETER_LEX_H

#define NAME_LENGTH 32

typedef enum{} lex_t;

class Lex{
    lex_t type;
    char name[NAME_LENGTH];
public:
    Lex();
    Lex(Lex &);
    ~Lex();

};



#endif //INTERPRETER_LEX_H
