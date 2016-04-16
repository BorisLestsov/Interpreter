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

void Scanner::print_vec(){
    vector<Lex>::iterator ptr;

    cout << "LEXEMS:" << endl;
    ptr = lex_vec.begin();
    while(ptr != lex_vec.cend()){
        //cout << *ptr << endl;
        cout << setw(10) << lex_map[ptr->get_type()] << setw(15) << ptr->get_value() << endl;
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

    STATE = H_ST;
    do {
        gc();
        //usleep(250000);
        //cout << c << ' ' << STATE <<  endl;
        switch (STATE) {
            case H_ST:
                if (!(c == ' ' || c == '\n' || c == '\t' || c == '\r'))
                    if (isalpha(c)){
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
                else {
                    j = look(buffer, WORD_NAMES);
                    STATE = H_ST;
                    if (j != 0)
                        lex_vec.push_back(Lex(WORD_LEXEMS[j], j));
                    else {
                        j = ID_TABLE.append(buffer);
                        lex_vec.push_back(Lex(LEX_ID, j));
                    }
                }
                break;
            case NUMB_ST:
                if(isdigit(c))
                    d = d * 10 + (c - '0');
                else
                    STATE = H_ST;
                    lex_vec.push_back(Lex (LEX_NUM, d));
                break;
            default:
                return;
        }
    } while (true);
}


//tables
const string Scanner::WORD_NAMES[] = {
        "",
        "and",
        "begin",
        "bool",
        "do",
        "else",
        "end",
        "if",
        "false",
        "int",
        "not",
        "or",
        "program",
        "read",
        "then",
        "true",
        "var",
        "while",
        "write",
        TBL_END
};

const string Scanner::DEL_NAMES[] = {
        "",
        "@",
        ";",
        ",",
        ":",
        ":=",
        "(",
        ")",
        "=",    //поменять
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

const lex_t Scanner::WORD_LEXEMS[] = {
        LEX_NULL,
        LEX_AND,
        LEX_BEGIN,
        LEX_BOOL,
        LEX_DO,
        LEX_ELSE,
        LEX_END,
        LEX_IF,
        LEX_FALSE,
        LEX_INT,
        LEX_NOT,
        LEX_OR,
        LEX_PROGRAM,
        LEX_READ,
        LEX_THEN,
        LEX_TRUE,
        LEX_VAR,
        LEX_WHILE,
        LEX_WRITE,
        LEX_NULL
};

const lex_t Scanner::DEL_LEXEMS[] = {
        LEX_NULL,
        LEX_FIN,
        LEX_SEMICOLON,
        LEX_COMMA,
        LEX_COLON,
        LEX_ASSIGN,
        LEX_LPAREN,
        LEX_RPAREN,
        LEX_EQ,
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

string debug[]{
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
        "LEX_ASSIGN",
        "LEX_LPAREN",
        "LEX_RPAREN",
        "LEX_EQ",
        "LEX_LSS",
        "LEX_GTR",
        "LEX_PLUS",
        "LEX_MINUS",
        "LEX_TIMES",
        "LEX_SLASH",
        "LEX_LEQ",
        "LEX_NEQ",
        "LEX_GEQ",
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