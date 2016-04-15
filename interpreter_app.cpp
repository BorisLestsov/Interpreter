//
// Created by Boris on 10.04.2016.
//
#include "scanner.h"
#include <iostream>

//TODO: порасставлять const и &

int main(int argc, char* argv[]){
    //Scanner main_scanner(argv[1]);

    Lex l1;
    Lex l2(LEX_ID, Lex::IDs, 1);

    cout << l1 << endl << l2 << endl;

    return 0;
}