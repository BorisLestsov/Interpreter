//
// Created by Boris on 10.04.2016.
//

#include <iostream>
#include "scanner.h"

using namespace std;

ID_table_t Scanner::ID_TABLE;

Scanner::Scanner(): lex_vec(0, Lex()) {
    clear_buffer();
    STATE = H_ST;
}

Scanner::Scanner(const char* input_f): lex_vec(0, Lex()){
    f = fopen(input_f, "r");
    clear_buffer();
    STATE = H_ST;
}

inline void Scanner::gc(){
    c = (char) fgetc(f);
}

inline void Scanner::addc(){
    buffer += c;
}

void Scanner::add_lex(Lex& cur_lex){
    lex_vec.push_back(cur_lex);
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

int Scanner::look(const string buf, const string table[]){
    int i = 0;

    while(table[i].compare(TBL_END)){
        if(!buf.compare(table[i])) return i;
        ++i;
    }
    return 0;
}

void Scanner::start(){
    int d, j;

    STATE = H_ST; //в методичке не так
    do {
        gc();
        switch (STATE) {
            case H_ST:
                if (c == ' ' || c == '\n' || c == '\t' || c == '\r') {
                    gc();
                } else if (isalpha(c)){
                    STATE = ID_ST;
                    clear_buffer();
                    addc();
                } else if(isdigit(c)){
                    STATE = NUMB_ST;
                    d = c - '0';
                } else if(c == '{'){
                    STATE = COM_ST;
                } else if(c == ':' || c == '<' || c == '>'){
                    STATE = ALE_ST;
                    clear_buffer();
                    addc();
                } else if(c == '@'){
                    lex_vec.push_back(Lex(LEX_FIN));
                    return;
                } else if(c == '!'){
                    STATE = NEQ_ST;
                    clear_buffer();
                    addc();
                }
                break;
            case ID_ST:
                if (isalpha(c) || isdigit(c))
                    addc ();
                else
                if ((j = look(buffer, Lex::WORD_NAMES)) != 0)
                    lex_vec.push_back(Lex(Lex::WORD_LEXEMS[j], Lex::WORD_NAMES, j));
                else
                {
                    j = ID_TABLE.append(buffer);
                    lex_vec.push_back(Lex (LEX_ID, NULL, j));
                }
                break;
            case NUMB_ST:
                if(isdigit(c))
                    d = d * 10 + (c - '0');
                else
                    lex_vec.push_back(Lex (LEX_NUM, NULL, d));
                break;
            default:
                return;
        }
    } while (true);
}