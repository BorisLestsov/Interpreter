//
// Created by Boris on 10.04.2016.
//

#include <iostream>
#include <unistd.h>
#include "Scanner.h"
#include <stack>

using namespace std;

ID_table_t Scanner::m_table;
vector<ID_table_t> Scanner::ID_tables_vec(1);

Scanner::Scanner(): lex_vec(0, Lex()) {
    clear_buffer();
    STATE = H_ST;
}

Scanner::Scanner(const char* input_f): lex_vec(0, Lex()){
    f = fopen(input_f, "r");
    if(f == NULL) throw Exception("Scanner error: could not open file");
    clear_buffer();
    STATE = H_ST;
}

Scanner::~Scanner(){
    fclose(f);
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

void Scanner::add_lex(lex_t type_par, int val_par, int add_val_par){
    lex_vec.push_back(Lex(type_par, val_par, add_val_par));
}

void Scanner::print_vec() const{
    vector<Lex>::const_iterator ptr;

    cout << "------LEXEMS:------" << endl;
    ptr = lex_vec.cbegin();
    while(ptr != lex_vec.cend()){
        //cout << *ptr << endl;
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

void Scanner::start() throw(exception){
    int d, j, sign = 1;
    char com_type;

    bool struct_name_defined = false;
    bool struct_flag = false;
    bool struct_names = false;
    int struct_index = 0;

    bool started = false;
    int M_STATE = MACRO_NULL;
    const ID* ID_ptr;
    enum m_skip_t {SKIP_ELSE, SKIP_IF};
    stack<m_skip_t> skip_stack;

    STATE = H_ST;
    do {
        gc();
        //usleep(250000);
        //cout << c << ' ';
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
                    gc();
                    if(c == '/') {
                        STATE = COM_ST;
                        com_type = '/';
                    } else if(c == '*') {
                        STATE = COM_ST;
                        com_type = '*';
                    } else {
                        add_lex(LEX_SLASH, LEX_SLASH);
                        ungetc(c,f);
                        STATE = H_ST;
                    }
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
                } else if (feof(f)) {
                    if(skip_stack.empty())
                        add_lex(LEX_FIN);
                    else throw Exception("Scanner error: expected endif macro", c);
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
                    if (j != 0) {
                        add_lex(WORD_LEXEMS[j], j);
                        if(WORD_LEXEMS[j] == LEX_STRUCT){
                            struct_flag = true;
                        }
                        if(struct_flag && WORD_LEXEMS[j] == LEX_END){
                            struct_flag = false;
                        }
                    } else {
                        ID_ptr = m_table.find(buffer);
                        if(ID_ptr == NULL){
                            if(!struct_name_defined && struct_flag){
                                struct_name_defined = true;
                                j = ID_tables_vec[0].append(buffer, LEX_STRUCT_T);
                                add_lex(LEX_ID, j);
                                ID_tables_vec.push_back(ID_table_t());
                                ++struct_index;

                                ID_tables_vec[struct_index].append(buffer, LEX_STRUCT_T);
                                ID_tables_vec[struct_index][0].set_value(j);

                                ID_tables_vec[0][j].set_value(struct_index);          // LEX_STRUCT_T's value in ID_tables_vec[0] is this structure's position in struct_vec
                            } else if(struct_name_defined && struct_flag){
                                j = ID_tables_vec[struct_index].append(buffer, LEX_ID);
                                add_lex(LEX_ID, j);
                            } else if (struct_names){
                                j = ID_tables_vec[0].append(buffer, LEX_STRUCT);
                                ID_tables_vec[0][j].set_value(struct_index);
                                add_lex(LEX_ID, j, 0);
                            } else {
                                if(lex_vec.back().get_type() == LEX_DOT){
                                    lex_vec.pop_back();
                                    if(lex_vec.back().get_type() != LEX_ID)
                                        throw Exception("Scanner error: expected struct name before: ", buffer);
                                    if(ID_tables_vec[0][lex_vec.back().get_value()].get_type() != LEX_STRUCT)
                                        throw Exception("Scanner error: expected struct name before: ", buffer);
                                    Lex tmp_lex = lex_vec.back();
                                    lex_vec.pop_back();
                                    j = ID_tables_vec[ID_tables_vec[0][tmp_lex.get_value()].get_value()].find_pos(buffer);
                                    if(j >= 0)
                                        add_lex(LEX_ID, j, ID_tables_vec[0]
                                                .find_pos(ID_tables_vec[0][tmp_lex.get_value()].get_name()));
                                    else throw Exception("Scanner error: unknown field in struct: ",
                                                         ID_tables_vec[0][tmp_lex.get_value()].get_name());
                                } else {
                                    j = ID_tables_vec[0].append(buffer, LEX_ID);
                                    add_lex(LEX_ID, j);
                                }
                            }
                        } else {
                            add_lex(LEX_NUM, ID_ptr->get_value());
                        }
                    }
                }
                break;
            case NUMB_ST:
                if (isdigit(c))
                    d = d * 10 + (c - '0');
                else {
                    d = d * sign;
                    if(M_STATE != MACRO_DEFINE){
                        add_lex(LEX_NUM, d);
                        STATE = H_ST;
                        ungetc(c, f);
                    } else {
                        ungetc(c, f);
                        STATE = ADD_MACRO;
                        M_STATE = DEFINE_FINISHED;
                    }
                }
                break;
            case SIGN_ST:
                if(isdigit(c)){
                    ungetc(c,f);
                    STATE = NUMB_ST;
                } else if(isalpha(c)){
                    if(sign > 0) add_lex(LEX_PLUS, LEX_PLUS);
                    else add_lex(LEX_MINUS, LEX_MINUS);
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
                    }else throw Exception("Scanner error: unknown literal: ", c);
                } else if(c == '\"' || feof(f)){
                    if(feof(f))
                        throw Exception("Scanner error: unclosed string");
                    if(!struct_flag)
                        j = ID_tables_vec[0].append(buffer + '\0', LEX_STRC); //that '\0' is cool
                    else
                        j = ID_tables_vec[struct_index].append(buffer, LEX_STRC);
                    add_lex(LEX_STRC, j);
                    STATE = H_ST;
                } else addc();
                break;
            case COM_ST:
                if(com_type == '/') {
                    if (c == '\n')
                        STATE = H_ST;
                } else if(c == '*' || feof(f)){
                    gc();
                    if(feof(f)) throw Exception("Scanner error: unclosed comment");
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
                    ungetc(c, f);
                    STATE = H_ST;
                }
                break;
            case EQ_ST:
                if(c == '=') {
                    add_lex(LEX_EQ, LEX_EQ);
                } else {
                    add_lex(LEX_ASSIGN, LEX_ASSIGN);
                    ungetc(c,f);
                }
                STATE = H_ST;
                break;
            case NEQ_ST:
                if(c == '='){
                    add_lex(LEX_NEQ, LEX_NEQ);
                } /*else {
                    add_lex(LEX_NOT);
                }*/ //TODO: !(a == b) - do I need it?
                STATE = H_ST;
                break;
            case DELIM_ST:
                j = look(buffer, DEL_NAMES);
                if(j != LEX_NULL){
                    if(struct_flag && j == 2){
                        struct_flag = false;
                        struct_name_defined = false;
                        struct_names = true;
                    } else if(j != 4)
                        struct_names = false;
                    add_lex(DEL_LEXEMS[j], j);
                    ungetc(c, f);
                } else {
                    addc();
                    j = look(buffer, DEL_NAMES);
                    if(j != LEX_NULL)
                        add_lex(DEL_LEXEMS[j], j);
                    else throw Exception("Scanner error: unknown word: ", buffer);
                }
                STATE = H_ST;
                break;
            case MACRO_ST:
                if(isalpha(c)){
                    addc();
                    M_STATE = look(buffer, MACRO_NAMES);
                    if(M_STATE != MACRO_NULL && M_STATE != DEFINE_FINISHED) {
                        clear_buffer();
                        STATE = ADD_MACRO;
                    }
                } else throw Exception("Scanner error: Unknown macro: ", buffer);
                break;
            case ADD_MACRO:
                switch (M_STATE) {
                    case MACRO_DEFINE:
                        if(started){
                            if(isalpha(c) || isdigit(c))
                                addc();
                            else {
                                if(m_table.find(buffer) != NULL)
                                    throw Exception("Scanner error: redefines unavaible", c);
                                gc();
                                sign = 1;
                                if(c == '-'){ sign = -1; gc();}
                                if(c == '+') gc();
                                if(!isdigit(c)) throw Exception("Scanner error: expected number");
                                else d = c - '0';
                                STATE = NUMB_ST;
                                started = false;
                            }
                        } else {
                            if(c != ' ' && c != '\t')
                                throw Exception("Scanner error: define name must be identifier", c);
                            gc();
                            if(isalpha(c)) {
                                addc();
                                started = true;
                            } else throw Exception("Scanner error: define name must be identifier", c);
                        }
                        break;
                    case MACRO_IFDEF:
                        if(started){
                            if(isalpha(c) || isdigit(c))
                                addc();
                            else {
                                if(c != '\n') throw Exception("Scanner error: ifdef expected \\n");
                                if(m_table.find(buffer) != NULL){
                                    STATE = H_ST;
                                    skip_stack.push(SKIP_ELSE);
                                } else {
                                    M_STATE = MACRO_SKIP;
                                    skip_stack.push(SKIP_IF);
                                }
                                started = false;
                            }
                        } else {
                            if(c != ' ' && c != '\t')
                                throw Exception("Scanner error: define name must be identifier", c);
                            gc();
                            if(isalpha(c)) {
                                addc();
                                started = true;
                            } else throw Exception("Scanner error: define name must be identifier", c);
                        }
                        break;
                    case MACRO_SKIP:
                        if(c == '#'){
                            clear_buffer();
                            STATE = MACRO_ST;
                        }
                        break;
                    case MACRO_IFNDEF:
                        if(started){
                            if(isalpha(c) || isdigit(c))
                                addc();
                            else {
                                if(c != '\n') throw Exception("Scanner error: ifndef expected \\n");
                                if(m_table.find(buffer) == NULL){
                                    STATE = H_ST;
                                    //skip = SKIP_ELSE;
                                    skip_stack.push(SKIP_ELSE);
                                } else {
                                    M_STATE = MACRO_SKIP;
                                    //skip = SKIP_IF;
                                    skip_stack.push(SKIP_IF);
                                }
                                started = false;
                            }
                        } else {
                            if(c != ' ' && c != '\t')
                                throw Exception("Scanner error: define name must be identifier", c);
                            gc();
                            if(isalpha(c)) {
                                addc();
                                started = true;
                            } else throw Exception("Scanner error: define name must be identifier", c);
                        }
                        break;
                    case MACRO_ELSE:
                        if(skip_stack.top() == SKIP_ELSE){
                            M_STATE = MACRO_SKIP;
                        } else {
                            STATE = H_ST;
                        }
                        break;
                    case MACRO_ENDIF:
                        skip_stack.pop();
                        STATE = H_ST;
                        M_STATE = MACRO_NULL;
                        break;
                    case MACRO_UNDEF:
                        if(started){
                            if(isalpha(c) || isdigit(c))
                                addc();
                            else {
                                if(c != '\n') throw Exception("Scanner error: undef expected \\n");
                                if(m_table.find(buffer) == NULL)
                                    throw Exception("Scanner error: identifier not found: ", buffer);
                                m_table.erase(buffer);
                                STATE = H_ST;
                                M_STATE = MACRO_NULL;
                                started = false;
                            }
                        } else {
                            if(c != ' ' && c != '\t')
                                throw Exception("Scanner error: define name must be identifier", c);
                            gc();
                            if(isalpha(c)) {
                                addc();
                                started = true;
                            } else if(isdigit(c)) throw Exception("Scanner error: define name must be identifier", c);
                        }
                        break;
                    case DEFINE_FINISHED:
                        if(c != '\n') throw Exception("Scanner error: define expected \\n");
                        M_STATE = MACRO_NULL;
                        STATE = H_ST;
                        m_table.append(buffer, LEX_MACRO_NAME, d);
                        break;
                }
                break;
        }
    } while (true);
}

//tables
const string Scanner::WORD_NAMES[] = {
        "",
        "and",
        "bool",
        "else",
        "if",
        "false",
        "int",
        "string",
        "not",
        "or",
        "program",
        "read",
        "true",
        "var",
        "while",
        "for",
        "write",
        "struct",
        "goto",
        "break",
        "continue",
        TBL_END
};

const string Scanner::DEL_NAMES[] = {
        "",
        "{",
        "}",
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
        ".",
        TBL_END
};

const string Scanner::MACRO_NAMES[] = {
        "",
        "define",
        "ifdef",
        "ifndef",
        "else",
        "endif",
        "undef",
        TBL_END
};

const lex_t Scanner::WORD_LEXEMS[] = {
        LEX_NULL,
        LEX_AND,
        LEX_BOOL,
        LEX_ELSE,
        LEX_IF,
        LEX_FALSE,
        LEX_INT,
        LEX_STRING,
        LEX_NOT,
        LEX_OR,
        LEX_PROGRAM,
        LEX_READ,
        LEX_TRUE,
        LEX_VAR,
        LEX_WHILE,
        LEX_FOR,
        LEX_WRITE,
        LEX_STRUCT,
        LEX_GOTO,
        LEX_BREAK,
        LEX_CONTINUE,
        LEX_MACRO_NAME,
        LEX_NULL
};

const lex_t Scanner::DEL_LEXEMS[] = {
        LEX_NULL,
        LEX_BEGIN,
        LEX_END,
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
        LEX_DOT,
        LEX_NULL
};
