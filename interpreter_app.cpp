//
// Created by Boris on 10.04.2016.
//
#include "scanner.h"
#include "parser.h"

using namespace std;

int main(int argc, char* argv[]){
    try {
        if(argc < 2) throw Exception("Interpreter error: no file");
        Scanner main_scanner(argv[1]);
        Lex::construct_lex_map();

        main_scanner.start();
        main_scanner.print_vec();
        main_scanner.ID_table.print_table();
        cout << "--------STRUCTURES--------" << endl;
        int i;
        for(i = 0; i < main_scanner.STRUCT_vec.size(); ++i){
            main_scanner.STRUCT_vec[i].print_table();
        }

        cout << "$$$$$$$$$$ PARSER $$$$$$$$$$$" << endl;
        Parser main_parser(main_scanner.get_lex_vec(), main_scanner.ID_table, main_scanner.STRUCT_vec);
        main_parser.start();
        main_scanner.ID_table.print_table();
        cout << "--------STRUCTURES--------" << endl;
        for(i = 0; i < main_scanner.STRUCT_vec.size(); ++i){
            main_scanner.STRUCT_vec[i].print_table();
        }
        cout << "$$$$$$$$$$ PROG $$$$$$$$$$$" << endl;
        main_parser.prog.print();
    }
    catch (exception& ex){
        cout << ex.what() << endl;
    }
    return 0;
}