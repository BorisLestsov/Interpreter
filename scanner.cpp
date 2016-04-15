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

    STATE = H_ST;
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
                if ((j = look(buffer, WORD_NAMES)) != 0)
                    lex_vec.push_back(Lex(WORD_LEXEMS[j], j));
                else
                {
                    j = ID_TABLE.append(buffer);
                    lex_vec.push_back(Lex(LEX_ID, j));
                }
                break;
            case NUMB_ST:
                if(isdigit(c))
                    d = d * 10 + (c - '0');
                else
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