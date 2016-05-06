//
// Created by Boris on 10.04.2016.
//
#include "scanner1.h"
#include "parser1.h"

using namespace std;

int main(int argc, char* argv[]){
    try {
        if(argc < 2) throw Exception("Interpreter error: no file");
        Scanner main_scanner(argv[1]);
        Lex::construct_lex_map();

        main_scanner.start();
        main_scanner.print_vec();
        cout << "--------TABLES--------" << endl;
        int i;
        for(i = 0; i < main_scanner.ID_tables_vec.size(); ++i){
            main_scanner.ID_tables_vec[i].print_table();
        }

        cout << "$$$$$$$$$$ PARSER $$$$$$$$$$$" << endl;
        Parser main_parser(main_scanner.lex_vec, main_scanner.ID_tables_vec);
        main_parser.start();
        cout << "--------TABLES--------" << endl;
        for(i = 0; i < main_scanner.ID_tables_vec.size(); ++i){
            main_scanner.ID_tables_vec[i].print_table();
        }
        cout << "$$$$$$$$$$ PROG $$$$$$$$$$$" << endl;
        main_parser.prog.print();
    }
    catch (exception& ex){
        cout << ex.what() << endl;
        return -5;
    }
    return 0;
}