//
// Created by Boris on 10.04.2016.
//

#include <iostream>
#include <unistd.h>
#include "scanner.h"

using namespace std;

ID_table_t Scanner::ID_TABLE;

Scanner::Scanner(): lex_vec(0, Lex()) {
    clear_buffer();
    STATE = H_ST;
}

Scanner::Scanner(const char* input_f): lex_vec(0, Lex()){
    f = fopen(input_f, "r");
    if(f == NULL) throw -1;
    clear_buffer();
    STATE = H_ST;
}

inline void Scanner::gc(){
    c = (char) fgetc(f);
}

inline void Scanner::addc(){
    buffer += c;
}

inline void Scanner::addc(char my_c){
    buffer += my_c;
}

void Scanner::add_lex(lex_t type_par, int val_par){
    lex_vec.push_back(Lex(type_par, val_par));
}

void Scanner::print_vec() const{
    vector<Lex>::const_iterator ptr;

    cout << "LEXEMS:" << endl;
    ptr = lex_vec.cbegin();
    while(ptr != lex_vec.cend()){
        //cout << *ptr << endl;
        cout << setw(15) << lex_map[ptr->get_type()] << setw(15) << ptr->get_value() << endl;
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

void Scanner::start() throw(exception){
    int d, j, sign = 1;
    char com_type;

    STATE = H_ST;
    do {
        gc();
        //usleep(250000);
        cout << c << ' ' << STATE << endl;
        switch (STATE) {
            case H_ST:
                if (c == ' ' || c == '\n' || c == '\t' || c == '\r') break;
                if (isalpha(c)) {
                    STATE = ID_ST;
                    clear_buffer();
                    addc();
                } else if (isdigit(c)) {
                    sign = 1;
                    STATE = NUMB_ST;
                    d = c - '0';
                } else if (c == '\"') {
                    STATE = STR_ST;
                    clear_buffer();
                } else if(c == '+' || c == '-'){
                    if(c == '+')
                        sign = 1;
                    else
                        sign = -1;
                    d = 0;
                    STATE = SIGN_ST;
                } else if (c == '/') {
                    STATE = COM_ST;
                    gc();
                    if(c == '/') {
                        com_type = '/';
                    } else if(c == '*') {
                        com_type = '*';
                    } else throw my_except();
                } else if (c == '!') {
                    STATE = NEQ_ST;
                    clear_buffer();
                    addc();
                } else if (c == '<' || c == '>') {
                    STATE = ALE_ST;
                    clear_buffer();
                    addc();
                } else if (c == '=') {
                    STATE = EQ_ST;
                    clear_buffer();
                    addc();
                    break;
                } else if (c == '@') {
                    add_lex(LEX_FIN);
                    return;
                } else if (c == '#') {
                    clear_buffer();
                    STATE = MACRO_ST;
                } else {
                    clear_buffer();
                    addc();
                    STATE = DELIM_ST;
                }
                break;
            case ID_ST:
                if (isalpha(c) || isdigit(c))
                    addc();
                else {
                    j = look(buffer, WORD_NAMES);
                    STATE = H_ST;
                    ungetc(c, f);
                    if (j != 0)
                        add_lex(WORD_LEXEMS[j], j);
                    else {
                        j = ID_TABLE.append(buffer, LEX_ID);
                        add_lex(LEX_ID, j);
                    }
                }
                break;
            case NUMB_ST:
                if (isdigit(c))
                    d = d * 10 + (c - '0');
                else {
                    STATE = H_ST;
                    ungetc(c, f);
                    d = d * sign;
                    add_lex(LEX_NUM, d);
                }
                break;
            case SIGN_ST:
                if(isdigit(c)){
                    ungetc(c,f);
                    STATE = NUMB_ST;
                } else if(isalpha(c)){
                    clear_buffer();
                    addc();
                    STATE = ID_ST;
                } else {
                    if(sign > 0) add_lex(LEX_PLUS, LEX_PLUS);
                    else add_lex(LEX_MINUS, LEX_MINUS);
                    STATE = H_ST;
                }
                break;
            case STR_ST:
                if(c == '\\') {
                    gc();
                    if(c == 'n' || c == 't' || c == '\"' || c == '\?' || c == '\\' || c == '\'') {
                        if (c == 'n') addc('\n');
                        else if(c == 't') addc('\t');
                        else addc();
                    }else throw my_except();
                } else if(c == '\"'){
                    j = ID_TABLE.append(buffer, LEX_STRC);
                    add_lex(LEX_STRC, j);
                    STATE = H_ST;
                } else if(c == '@'){
                    throw my_except();
                } else addc();
                break;
            case COM_ST:
                if(com_type == '/') {
                    if (c == '\n')
                        STATE = H_ST;
                } else if(c == '*'){
                    gc();
                    if(c == '/')
                        STATE = H_ST;
                }
                break;
            case ALE_ST:
                if(c == '=') {
                    addc();
                    j = look(buffer, DEL_NAMES);
                    add_lex(DEL_LEXEMS[j], j);
                    STATE = H_ST;
                } else {
                    j = look(buffer, DEL_NAMES);
                    add_lex(DEL_LEXEMS[j], j);
                    STATE = H_ST;
                }
                break;
            case EQ_ST:
                if(c == '=') {
                    add_lex(LEX_EQ, LEX_EQ);
                } else {
                    add_lex(LEX_ASSIGN, LEX_ASSIGN);
                }
                STATE = H_ST;
                break;
            case NEQ_ST:
                if(c == '='){
                    add_lex(LEX_NEQ, LEX_NEQ);
                } /*else {
                    add_lex(LEX_NOT);
                }*/ //TODO: what should it be?
                STATE = H_ST;
                break;
            case DELIM_ST:
                j = look(buffer, DEL_NAMES);
                if(j != LEX_NULL){
                    add_lex(DEL_LEXEMS[j], j);
                    ungetc(c, f);
                } else {
                    addc();
                    j = look(buffer, DEL_NAMES);
                    if(j != LEX_NULL)
                        add_lex(DEL_LEXEMS[j], j);
                    else throw my_except();
                }
                STATE = H_ST;
                break;
            case MACRO_ST:
                if(isalpha(c)){
                    addc();
                    j = look(buffer, MACRO_NAMES);
                    if(j != LEX_NULL) {
                        STATE = ADD_MACRO;
                    }
                } else throw my_except();
                break;
            case ADD_MACRO:
                if(isalpha(c)){
                    addc();
                } else {}

                break;
        }
    } while (true);
}

//tables
const string Scanner::WORD_NAMES[] = {
        "",
        "and",
        "bool",
        "do",
        "else",
        "if",
        "false",
        "int",
        "string",
        "not",
        "or",
        "program",
        "read",
        "then",
        "true",
        "var",
        "while",
        "write",
        "struct",
        TBL_END
};

const string Scanner::DEL_NAMES[] = {
        "",
        "{",
        "}",
        "@",
        ";",
        ",",
        ":",
        "==",
        "(",
        ")",
        "=",
        "<",
        ">",
        "+",
        "-",
        "*",
        "/",
        "<=",
        "!=",
        ">=",
        TBL_END
};

const string Scanner::MACRO_NAMES[] = {
        "",
        "define",
        "ifdef",
        "ifndef",
        TBL_END
};

const lex_t Scanner::WORD_LEXEMS[] = {
        LEX_NULL,
        LEX_AND,
        LEX_BOOL,
        LEX_DO,
        LEX_ELSE,
        LEX_IF,
        LEX_FALSE,
        LEX_INT,
        LEX_STRING,
        LEX_NOT,
        LEX_OR,
        LEX_PROGRAM,
        LEX_READ,
        LEX_THEN,
        LEX_TRUE,
        LEX_VAR,
        LEX_WHILE,
        LEX_WRITE,
        LEX_STRUCT,
        LEX_NULL
};

const lex_t Scanner::DEL_LEXEMS[] = {
        LEX_NULL,
        LEX_BEGIN,
        LEX_END,
        LEX_FIN,
        LEX_SEMICOLON,
        LEX_COMMA,
        LEX_COLON,
        LEX_EQ,
        LEX_LPAREN,
        LEX_RPAREN,
        LEX_ASSIGN,
        LEX_LSS,
        LEX_GTR,
        LEX_PLUS,
        LEX_MINUS,
        LEX_TIMES,
        LEX_SLASH,
        LEX_LEQ,
        LEX_NEQ,
        LEX_GEQ,
        LEX_NULL
};

//посмотреть результат

string debug[] = {
        "LEX_NULL",
        "LEX_AND",
        "LEX_BEGIN",
        "LEX_BOOL",
        "LEX_DO",
        "LEX_ELSE",
        "LEX_END",
        "LEX_IF",
        "LEX_FALSE",
        "LEX_INT",
        "LEX_STRING",
        "LEX_NOT",
        "LEX_OR",
        "LEX_PROGRAM",
        "LEX_READ",
        "LEX_THEN",
        "LEX_TRUE",
        "LEX_VAR",
        "LEX_WHILE",
        "LEX_WRITE",
        "LEX_FIN",
        "LEX_SEMICOLON",
        "LEX_COMMA",
        "LEX_COLON",
        "LEX_EQ",
        "LEX_LPAREN",
        "LEX_RPAREN",
        "LEX_ASSIGN",
        "LEX_LSS",
        "LEX_GTR",
        "LEX_PLUS",
        "LEX_MINUS",
        "LEX_TIMES",
        "LEX_SLASH",
        "LEX_LEQ",
        "LEX_NEQ",
        "LEX_GEQ",
        "LEX_STRC",
        "LEX_STRUCT",
        "LEX_NUM",
        "LEX_ID",
};

map<lex_t, string> Scanner::lex_map;

void Scanner::construct_lex_map(){
    lex_t i;
    /*lex_map.insert(make_pair(LEX_NULL, WORD_NAMES[0]));
    lex_map.insert(make_pair(LEX_NUM, "LEX_NUM"));
    lex_map.insert(make_pair(LEX_ID, "LEX_ID"));
    for(i = 1; WORD_LEXEMS[i] != LEX_NULL; ++i)
        lex_map.insert(make_pair(WORD_LEXEMS[i], WORD_NAMES[i]));
    for(i = 1; DEL_LEXEMS[i] != LEX_NULL; ++i)
        lex_map.insert(make_pair(DEL_LEXEMS[i], DEL_NAMES[i]));*/
    for(i = LEX_NULL; i != LEX_ID; ++i){
        lex_map.insert(make_pair(i , debug[i]));
    }
    lex_map.insert(make_pair(i, debug[i]));
}