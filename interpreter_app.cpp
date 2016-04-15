//
// Created by Boris on 10.04.2016.
//
#include "scanner.h"

//TODO: порасставлять const и &

int main(){
    Scanner main_scanner("prog");

    cout << main_scanner.ID_TABLE.append("hello") << endl;
    cout << main_scanner.ID_TABLE.append("hel") << endl;
    cout << main_scanner.ID_TABLE.append("hello") << endl;

    main_scanner.ID_TABLE.print_table();

    return 0;
}