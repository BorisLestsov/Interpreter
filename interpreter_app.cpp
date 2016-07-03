//
// Created by Boris on 10.04.2016.
//
#include "units/Scanner.h"
#include "units/Parser.h"
#include "units/Virtual_Machine.h"

using namespace std;

int DEBUG_MODE = 0;
int SCANNER_DELAY_TIME = 0;

int main(int argc, char* argv[]){
    try {
        if(argc < 2) throw Exception("Interpreter error: no file");

        int i;

        Scanner main_scanner(argv[1]);
        Lex::construct_lex_map();

        main_scanner.start();
        if (DEBUG_MODE){
            main_scanner.print_vec();
            cout << "--------TABLES--------" << endl;
            for (i = 0; i < main_scanner.ID_tables_vec.size(); ++i) {
                main_scanner.ID_tables_vec[i].print_table();
            }
        }
        if (DEBUG_MODE)
            cout << "$$$$$$$$$$ PARSER $$$$$$$$$$$" << endl;

        Parser main_parser(main_scanner.lex_vec, main_scanner.ID_tables_vec);
        main_parser.start();

        if (DEBUG_MODE) {
            cout << "--------TABLES--------" << endl;
            for (i = 0; i < main_scanner.ID_tables_vec.size(); ++i) {
                main_scanner.ID_tables_vec[i].print_table();
            }
            cout << "$$$$$$$$$$ PROG $$$$$$$$$$$" << endl;
            main_parser.prog.print();
        }

        if (DEBUG_MODE)
            cout << "$$$$$$$$$$ VIRTUAL MACHINE $$$$$$$$$$$" << endl;

        Virtual_Machine VM(main_parser.prog, main_parser.ID_tables_vec);
        VM.start();
    }
    catch (exception& ex){
        cout << ex.what() << endl;
        return -1;
    }
    return 0;
}