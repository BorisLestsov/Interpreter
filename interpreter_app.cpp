//
// Created by Boris on 10.04.2016.
//
#include "scanner.h"

int main(int argc, char* argv[]){
    try {
        if(argc < 2) throw -1;
        Scanner main_scanner(argv[1]);
        main_scanner.construct_lex_map();

        main_scanner.start();
        main_scanner.print_vec();
        main_scanner.ID_TABLE.print_table();
    }
    catch (int){ return -1;}
    catch (char c){cerr << "wrong symbol: " << c <<endl; return -1;}
    return 0;
}