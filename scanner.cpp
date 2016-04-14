//
// Created by Boris on 10.04.2016.
//

#include <iostream>
#include "scanner.h"


using namespace std;

Scanner::Scanner(const char* input_f): lex_vec(0, Lex()), buffer(){
    f = fopen(input_f, "r");
    clear_buffer();
    // TODO сдесь должен быть gc();
}

void Scanner::add_lex(Lex& cur_lex){
    lex_vec.push_back(cur_lex);
}
Lex Scanner::get_last_lex(){
    return lex_vec.back();
}

void Scanner::print_vec(){
    vector<Lex>::iterator ptr;

    ptr = lex_vec.begin();
    while(ptr != lex_vec.cend()){
        cout << *ptr << endl;
        ptr++;
    }
}

void Scanner::clear_buffer(){
    buffer.clear();
}