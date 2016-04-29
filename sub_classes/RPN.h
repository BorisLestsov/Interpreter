//
// Created by boris on 29.04.16.
//
#pragma once

#include "Lex.h"
#include <iostream>
#include <vector>

using namespace std;

//Reverse Polish Notation
class RPN: public vector<Lex> {
public:
    void print() const;
    void blank();
    int get_pos() const;
};
