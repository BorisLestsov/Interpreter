//
// Created by Boris on 10.04.2016.
//

#include "parser.h"

Parser::Parser(const vector<Lex>& lex_vec_par, ID_table_t& ID_table_par, vector<ID_table_t>& STRUCT_vec_par):
        lex_vec(lex_vec_par),
        ID_table(ID_table_par),
        STRUCT_vec(STRUCT_vec_par)
{
    index = lex_vec.cbegin();
}

void Parser::get_lex() {
    c_lex = *index;
    c_type = c_lex.get_type();
    c_val = c_lex.get_value();
    ++index;
    cout << c_lex << endl;
}

void Parser::make_tmp() {
    tmp_type = c_type;
    tmp_val = c_val;
}


void Parser::start() {
    PROGRAM();
}

void Parser::PROGRAM(){
    get_lex();
    if(c_type == LEX_PROGRAM)
        DECLARATIONS();
    else throw Exception("Parser error: expected \"program\" but got lexem: ", Lex::lex_map[c_type]);
    OPERATORS();
    /*if(c_type != LEX_FIN)
        throw Exception("Parser error: unexpected lexem: ", Lex::lex_map[c_type]);*/

}

void Parser::DECLARATIONS() {
    get_lex();
    if(c_type != LEX_BEGIN)
        throw Exception("Parser error: expected { but got lexem: ", Lex::lex_map[c_type]);
    //push { to stack?

    get_lex();
    while(c_type == LEX_STRUCT) {
        STRUCT_DECL();
        get_lex();
    }

    //get_lex();
    while(c_type == LEX_INT || c_type == LEX_BOOL || c_type == LEX_STRING){
        DECL();
        get_lex();
    }
}

void Parser::STRUCT_DECL() {
    int cur_struct_index;

    get_lex();
    if(c_type == LEX_ID) {
        cur_struct_index = ID_table[c_val].get_value();
    } else throw Exception("Parser error: expecded ID but got lexem: ", Lex::lex_map[c_type]);
    get_lex();
    if(c_type != LEX_BEGIN) throw Exception("Parser error: expected { but got lexem: ", Lex::lex_map[c_type]);
    get_lex();
    while(c_type == LEX_INT || c_type == LEX_BOOL || c_type == LEX_STRING){
        SWITCH_ID(STRUCT_vec[cur_struct_index]);
        get_lex();
    }
    if(c_type != LEX_END) throw Exception("Parser error: unexpected lexem: ", Lex::lex_map[c_type]);
    get_lex();
    while(c_type != LEX_SEMICOLON){
        if(c_type == LEX_ID){
            ID_table[c_val].set_type(LEX_STRUCT);
            ID_table[c_val].set_val(cur_struct_index);
            ID_table[c_val].set_declared(true);
        } else throw Exception("Parser error: unexpected lexem: ", Lex::lex_map[c_type]);
        get_lex();
        if(c_type == LEX_COMMA)
            get_lex();
        if(c_type != LEX_ID && c_type != LEX_SEMICOLON)
            throw Exception("Parser error: unexpected lexem: ", Lex::lex_map[c_type]);
    }
}

void Parser::SWITCH_ID(ID_table_t& table) {
    switch (c_type) {
        case LEX_INT:
            while(c_type != LEX_SEMICOLON){
                get_lex();
                if(c_type == LEX_ID) {
                    if(table.multiple_declaration(table[c_val].get_name()))
                        throw Exception("Parser error: multiple declaration of : ", table[c_val].get_name());
                    table[c_val].set_declared(true);
                    table[c_val].set_type(LEX_INT);
                } else throw Exception("Parser error: expected ID but got lexem: ", Lex::lex_map[c_type]);
                make_tmp();
                get_lex();
                if(c_type == LEX_ASSIGN) {
                    get_lex();
                    if (c_type == LEX_NUM) {
                        table[tmp_val].set_assigned(true);
                        table[tmp_val].set_val(c_val);
                    } else throw Exception("Parser error: expected number but got lexem: ", Lex::lex_map[c_type]);
                    get_lex();
                } else
                    table[tmp_val].set_assigned(false);
                if(c_type == LEX_COMMA || c_type == LEX_SEMICOLON)
                    continue;
                else throw Exception("Parser error: unexpected lexem: ", Lex::lex_map[c_type]);
            }
            break;
        case LEX_BOOL:
            while(c_type != LEX_SEMICOLON){
                get_lex();
                if(c_type == LEX_ID) {
                    if(table.multiple_declaration(table[c_val].get_name()))
                        throw Exception("Parser error: multiple declaration of : ", table[c_val].get_name());
                    table[c_val].set_declared(true);
                    table[c_val].set_type(LEX_BOOL);
                } else throw Exception("Parser error: expected ID but got lexem: ", Lex::lex_map[c_type]);
                make_tmp();
                get_lex();
                if(c_type == LEX_ASSIGN) {
                    get_lex();
                    if (c_type == LEX_TRUE) {
                        table[tmp_val].set_assigned(true);
                        table[tmp_val].set_val(1);
                    } else if(c_type == LEX_FALSE){
                        table[tmp_val].set_assigned(true);
                        table[tmp_val].set_val(0);
                    } else
                        throw Exception("Parser error: expected bool constant but get lexem: ", Lex::lex_map[c_type]);
                    get_lex();
                } else
                    table[tmp_val].set_assigned(false);
                if(c_type == LEX_COMMA || c_type == LEX_SEMICOLON)
                    continue;
                else throw Exception("Parser error: unexpected lexem: ", Lex::lex_map[c_type]);
            }
            break;
        case LEX_STRING:
            while(c_type != LEX_SEMICOLON){
                get_lex();
                if(c_type == LEX_ID) {
                    if(table.multiple_declaration(table[c_val].get_name()))
                        throw Exception("Parser error: multiple declaration of : ", table[c_val].get_name());
                    table[c_val].set_declared(true);
                    table[c_val].set_type(LEX_STRING);
                } else throw Exception("Parser error: expected ID but got lexem: ", Lex::lex_map[c_type]);
                make_tmp();
                get_lex();
                if(c_type == LEX_ASSIGN){
                    get_lex();
                    if(c_type == LEX_STRC){
                        table[tmp_val].set_assigned(true);
                        table[tmp_val].set_val(c_val);
                    } else throw Exception("Parser error: expected string constant but got lexem: ", Lex::lex_map[c_type]);
                    get_lex();
                } else
                    table[tmp_val].set_assigned(false);
                if(c_type == LEX_COMMA || c_type == LEX_SEMICOLON)
                    continue;
                else throw Exception("Parser error: unexpected lexem: ", Lex::lex_map[c_type]);
            }
            break;
        default: throw Exception("Parser error: unexpected lexem: ", Lex::lex_map[c_type]);
    }
}

void Parser::DECL(){
    SWITCH_ID(ID_table);
}

void Parser::OPERATORS(){
    while(c_type != LEX_FIN){
        OP();
    }
}

void Parser::OP(){
    int pl0, pl1, pl2, pl3;

    switch (c_type) {
        case LEX_IF:
            get_lex();
            EXPRESSION();
            eq_bool();
            /*pl2 = prog.get_free ();
            prog.blank();
            prog.put_lex (Lex(POLIZ_FGO));*/
            if (c_type == LEX_THEN) {
                get_lex();
                OP();
                /*pl3 = prog.get_free();
                prog.blank();
                prog.put_lex (Lex(POLIZ_GO));
                prog.put_lex (Lex(POLIZ_LABEL, prog.get_free()),pl2);*/
                if (c_type == LEX_ELSE) {
                    get_lex();
                    OP();
                    //prog.put_lex(Lex(POLIZ_LABEL,prog.get_free()),pl3);
                }
            } else
                throw Exception("Parser error: unexpected lexem: ", Lex::lex_map[c_type]);
            break;
            //end if
        case LEX_WHILE:
            //pl0 = prog.get_free();
            get_lex();
            EXPRESSION();
            eq_bool();
            /*pl1 = prog.get_free();
            prog.blank();
            prog.put_lex(Lex(POLIZ_FGO));*/
            if (c_type == LEX_DO) {
                get_lex();
                OP();
                /*prog.put_lex(Lex(POLIZ_LABEL, pl0));
                prog.put_lex(Lex(POLIZ_GO));
                prog.put_lex(Lex(POLIZ_LABEL, prog.get_free()), pl1);*/
            }
            else
                throw Exception("Parser error: unexpected lexem: ", Lex::lex_map[c_type]);
            break; //end while
        case LEX_READ:
                get_lex();
                if (c_type == LEX_LPAREN) {
                    get_lex();
                    if (c_type == LEX_ID) {
                        check_id_in_read();
                        //prog.put_lex(Lex(POLIZ_ADDRESS, c_val));
                        get_lex();
                    }
                    else
                        throw Exception("Parser error: expected ID but recieved : ", Lex::lex_map[c_type]);
                    if (c_type == LEX_RPAREN) {
                        get_lex();
                        //prog.put_lex(Lex(LEX_READ));
                    }
                    else
                        throw Exception("Parser error:  expected \")\" but recieved: ", Lex::lex_map[c_type]);
                }
                else
                    throw Exception("Parser error:  expected \"(\" but recieved: ", Lex::lex_map[c_type]);
            break; //end read
        case LEX_WRITE:
                get_lex();
                if (c_type == LEX_LPAREN) {
                    get_lex();
                    EXPRESSION();
                    if (c_type == LEX_RPAREN) {
                        get_lex();
                        //prog.put_lex(Lex(LEX_WRITE));
                    }
                    else
                        throw Exception("Parser error:  expected \")\" but recieved: ", Lex::lex_map[c_type]);
                }
                else
                    throw Exception("Parser error:  expected \"(\" but recieved: ", Lex::lex_map[c_type]);
            break; //end write
        case LEX_ID:
                check_id();
                //prog.put_lex(Lex(POLIZ_ADDRESS, c_val));
                get_lex();
                if (c_type == LEX_ASSIGN /* || c_type == LEX_COLON*/) {
                    // M: LEX_ID should be changed to LEX_LABEL and "DECLARED" must be set
                    get_lex();
                    EXPRESSION();
                    eq_type();
                    //prog.put_lex(Lex(LEX_ASSIGN));
                }
                else
                    throw Exception("Parser error: unxpected lexem: ", Lex::lex_map[c_type]);
            break; //assign-end
        case LEX_BEGIN:
            COMPLEX_OP();
            break;
        case LEX_SEMICOLON:
            get_lex();
            break;
        default:
            throw Exception("Parser error: /*default reached*/: unxpected lexem: ", Lex::lex_map[c_type]);
            break;
    }
}

void Parser::COMPLEX_OP(){
    if ( c_type == LEX_BEGIN )
    {
        get_lex();
        OP();
        while ( c_type == LEX_SEMICOLON )
        {
            get_lex();
            if(c_type == LEX_END) break;
            OP();
        }
        if ( c_type == LEX_END )
            get_lex();
        else
            throw Exception("Parser error: expected \"}\" but recieved: ", Lex::lex_map[c_type]);
    }
    else
        throw Exception("Parser error: expected \"{\" but recieved: ", Lex::lex_map[c_type]);
}

void Parser::EXPRESSION() {
    LOW();
    if ( c_type == LEX_EQ || c_type == LEX_LSS || c_type == LEX_GTR ||
         c_type == LEX_LEQ || c_type == LEX_GEQ || c_type == LEX_NEQ )
    {
        //st_lex.push (c_type);
        get_lex();
        LOW();
        check_op();
    }
}

void Parser::LOW() {
    HIGH();
    while ( c_type == LEX_PLUS || c_type == LEX_MINUS || c_type == LEX_OR )
    {
        //st_lex.push (c_type);
        get_lex();
        HIGH();
        check_op();
    }
}

void Parser::HIGH() {
    F();
    while ( c_type == LEX_TIMES || c_type == LEX_SLASH || c_type == LEX_AND )
    {
        //st_lex.push (c_type);
        get_lex();
        F();
        check_op();
    }
}

void Parser::F ()
{
    switch (c_type){
        case LEX_ID:
            check_id();
            //prog.put_lex (Lex (LEX_ID, c_val));
            get_lex();
            break;
        case LEX_NUM:
            //st_lex.push ( LEX_INT );
            //prog.put_lex ( curr_lex );
            get_lex();
        break;
        case LEX_TRUE:
            //st_lex.push ( LEX_BOOL );
            //prog.put_lex (Lex (LEX_TRUE, 1) );
            get_lex();
            break;
        case LEX_FALSE:
            //st_lex.push ( LEX_BOOL );
            //prog.put_lex (Lex (LEX_FALSE, 0) );
            get_lex();
            break;
        case LEX_NOT:
            get_lex();
            F();
            check_not();
            break;
        case LEX_LPAREN:
            get_lex();
            EXPRESSION();
            if ( c_type == LEX_RPAREN)
                get_lex();
            else
                throw Exception("Parser error: expected \"(\" but recieved: ", Lex::lex_map[c_type]);
            break;
        default: throw Exception("Parser error: unexpected lexem: ", Lex::lex_map[c_type]);
    }
}


//-------------symantics check:--------------
bool Parser::eq_bool() {
    return false;
}

bool Parser::eq_type() {
    return false;
}

bool Parser::check_id_in_read() {
    return false;
}

bool Parser::check_id() {
    return false;
}

bool Parser::check_op() {
    return false;
}

bool Parser::check_not() {
    return false;
}
