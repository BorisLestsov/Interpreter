//
// Created by boris on 29.04.16.
//

#include "RPN.h"

void RPN::print() const{
    vector<Lex>::const_iterator ptr = cbegin();
    vector<Lex>::const_iterator end = cend();
    int i = 0;

    while(ptr != end){
        cout << setw(2) << i++  << *ptr << endl;
        ++ptr;
    }
}

void RPN::blank() {
    push_back(Lex(LEX_NULL, 0));
}

int RPN::get_pos() const {
    return (int) size();
}
