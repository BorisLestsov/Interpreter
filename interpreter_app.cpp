//
// Created by Boris on 10.04.2016.
//
#include "scanner.h"

int main(int argc, char* argv[]){
    //try {
       // if(argc < 2) throw -1;
        Scanner main_scanner(argv[1]);

        main_scanner.start();

    //}
    //catch(int){ return -1;}
    return 0;
}