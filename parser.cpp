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
    else throw Exception("Parser error: expected \"program\" but recieved lexem: ", Lex::lex_map[c_type]);
    OPERATORS();
    /*if(c_type != LEX_FIN)
        throw Exception("Parser error: unexpected lexem: ", Lex::lex_map[c_type]);*/

}

void Parser::DECLARATIONS() {
    get_lex();
    if(c_type != LEX_BEGIN)
        throw Exception("Parser error: expected { but recieved lexem: ", Lex::lex_map[c_type]);
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
    } else throw Exception("Parser error: expecded ID but recieved lexem: ", Lex::lex_map[c_type]);
    get_lex();
    if(c_type != LEX_BEGIN) throw Exception("Parser error: expected { but recieved lexem: ", Lex::lex_map[c_type]);
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
                    if(table.multiple_declaration(c_val))
                        throw Exception("Parser error: multiple declaration of : ", table[c_val].get_name());
                    table[c_val].set_declared(true);
                    table[c_val].set_type(LEX_INT);
                } else throw Exception("Parser error: expected ID but recieved lexem: ", Lex::lex_map[c_type]);
                make_tmp();
                get_lex();
                if(c_type == LEX_ASSIGN) {
                    get_lex();
                    if (c_type == LEX_NUM) {
                        table[tmp_val].set_assigned(true);
                        table[tmp_val].set_val(c_val);
                    } else throw Exception("Parser error: expected number but recieved lexem: ", Lex::lex_map[c_type]);
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
                    if(table.multiple_declaration(c_val))
                        throw Exception("Parser error: multiple declaration of : ", table[c_val].get_name());
                    table[c_val].set_declared(true);
                    table[c_val].set_type(LEX_BOOL);
                } else throw Exception("Parser error: expected ID but recieved lexem: ", Lex::lex_map[c_type]);
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
                    if(table.multiple_declaration(c_val))
                        throw Exception("Parser error: multiple declaration of : ", table[c_val].get_name());
                    table[c_val].set_declared(true);
                    table[c_val].set_type(LEX_STRING);
                } else throw Exception("Parser error: expected ID but recieved lexem: ", Lex::lex_map[c_type]);
                make_tmp();
                get_lex();
                if(c_type == LEX_ASSIGN){
                    get_lex();
                    if(c_type == LEX_STRC){
                        table[tmp_val].set_assigned(true);
                        table[tmp_val].set_val(c_val);
                    } else throw Exception("Parser error: expected string constant but recieved lexem: ", Lex::lex_map[c_type]);
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
    while(c_type != LEX_FIN && c_type != LEX_END){
        OP();
    }
    get_lex();
    if (c_type != LEX_FIN)
        throw Exception("Parser error: expected LEX_FIN but recieved lexem: ", Lex::lex_map[c_type]);
}

void Parser::OP(){
    int blank_pos0, blank_pos1, blank_pos2, blank_pos3;

    switch (c_type) {
        case LEX_IF:
            get_lex();
            EXPRESSION();
            eq_bool();
            blank_pos2 = prog.get_pos ();
            prog.blank();
            prog.push_back(Lex(RPN_FGO));
            if (c_type == LEX_THEN) {
                get_lex();
                OP();
                blank_pos3 = prog.get_pos();
                prog.blank();
                prog.push_back(Lex(RPN_GO));
                prog[blank_pos2] = Lex(RPN_LABEL, prog.get_pos());
                if (c_type == LEX_ELSE) {
                    get_lex();
                    OP();
                    prog[blank_pos3] = Lex(RPN_LABEL,prog.get_pos());
                }
            } else
                throw Exception("Parser error: unexpected lexem: ", Lex::lex_map[c_type]);
            break;
        case LEX_WHILE:
            blank_pos0 = prog.get_pos();
            get_lex();
            EXPRESSION();
            eq_bool();
            blank_pos1 = prog.get_pos();
            prog.blank();
            prog.push_back(Lex(RPN_FGO));
            if (c_type == LEX_DO) {
                get_lex();
                OP();
                prog.push_back(Lex(RPN_LABEL, blank_pos0));
                prog.push_back(Lex(RPN_GO));
                prog[blank_pos1] = Lex(RPN_LABEL, prog.get_pos());
            }
            else
                throw Exception("Parser error: unexpected lexem: ", Lex::lex_map[c_type]);
            break;
        case LEX_READ:
            get_lex();
            if (c_type == LEX_LPAREN) {
                get_lex();
                if (c_type == LEX_ID) {
                    check_id();
                    prog.push_back(Lex(RPN_ADDRESS, c_val));
                    get_lex();
                }
                else
                    throw Exception("Parser error: expected ID but recieved : ", Lex::lex_map[c_type]);
                if (c_type == LEX_RPAREN) {
                    get_lex();
                    prog.push_back(Lex(LEX_READ));
                }
                else
                    throw Exception("Parser error:  expected \")\" but recieved: ", Lex::lex_map[c_type]);
            }
            else
                throw Exception("Parser error:  expected \"(\" but recieved: ", Lex::lex_map[c_type]);
            break;
        case LEX_WRITE:
            get_lex();
            if (c_type == LEX_LPAREN) {
                get_lex();
                EXPRESSION();
                if (c_type == LEX_RPAREN) {
                    get_lex();
                    prog.push_back(Lex(LEX_WRITE));
                }
                else
                    throw Exception("Parser error:  expected \")\" but recieved: ", Lex::lex_map[c_type]);
            }
            else
                throw Exception("Parser error:  expected \"(\" but recieved: ", Lex::lex_map[c_type]);
            break;
        case LEX_ID:
            make_tmp();
            get_lex();
            switch (c_type) {
                case LEX_ASSIGN:
                    if ( !(ID_table[tmp_val].get_declared()) )
                        throw Exception("Parser error: use of undeclared ID: ", ID_table[c_val].get_name());
                    prog.push_back(Lex(RPN_ADDRESS, tmp_val));
                    lex_stack.push(ID_table[tmp_val].get_type());
                    get_lex();
                    EXPRESSION();
                    check_assign();
                    prog.push_back(Lex(LEX_ASSIGN));
                    break;
                case LEX_COLON:
                    ID_table[tmp_val].set_type(LEX_LABEL);
                    if(ID_table.multiple_declaration(tmp_val))
                        throw Exception("Parser error: multiple declaration of Label: ", ID_table[tmp_val].get_name());
                    ID_table[tmp_val].set_val(prog.get_pos());
                    ID_table[tmp_val].set_declared(true);
                    c_type = LEX_SEMICOLON;
                    break;
                default:
                    throw Exception("Parser error: unxpected lexem: ", Lex::lex_map[c_type]);
            }
            break;
        case LEX_BEGIN:
            COMPLEX_OP();
            break;
        case LEX_SEMICOLON:
            get_lex();
            break;
        case LEX_BREAK:
            throw Exception("Parser error: IN DEVELOPEMENT: ", Lex::lex_map[c_type]);
            get_lex();
            break;
        case LEX_GOTO:
            throw Exception("Parser error: IN DEVELOPEMENT: ", Lex::lex_map[c_type]);
            get_lex();
            break;
        case LEX_FOR:
            throw Exception("Parser error: IN DEVELOPEMENT: ", Lex::lex_map[c_type]);
            get_lex();
            break;
        default:
            throw Exception("Parser error: Unknown operator: ", Lex::lex_map[c_type]);
            break;
    }
}

void Parser::COMPLEX_OP(){
    if ( c_type == LEX_BEGIN ) {
        do {
            get_lex();
            if(c_type == LEX_END) break;
            OP();
        } while  ( c_type == LEX_SEMICOLON );
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
        lex_stack.push (c_type);
        get_lex();
        LOW();
        check_op();
    }
}

void Parser::LOW() {
    HIGH();
    while ( c_type == LEX_PLUS || c_type == LEX_MINUS || c_type == LEX_OR ) {
        lex_stack.push(c_type);
        get_lex();
        HIGH();
        check_op();
    }
}

void Parser::HIGH() {
    F();
    while ( c_type == LEX_TIMES || c_type == LEX_SLASH || c_type == LEX_AND ) {
        lex_stack.push (c_type);
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
            lex_stack.push(ID_table[c_val].get_type());
            prog.push_back (Lex (LEX_ID, c_val));
            get_lex();
            break;
        case LEX_NUM:
            lex_stack.push ( LEX_INT );
            prog.push_back ( c_lex );
            get_lex();
        break;
        case LEX_TRUE:
            lex_stack.push ( LEX_BOOL );
            prog.push_back (Lex (LEX_TRUE, 1) );
            get_lex();
            break;
        case LEX_FALSE:
            lex_stack.push ( LEX_BOOL );
            prog.push_back (Lex (LEX_FALSE, 0) );
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
        case LEX_STRC:
            lex_stack.push ( LEX_STRC );
            prog.push_back ( c_lex );
            get_lex();
            break;
        default: throw Exception("Parser error: unexpected lexem: ", Lex::lex_map[c_type]);
    }
}


//-------------symantics check:--------------
void Parser::eq_bool() {
    if ( lex_stack.top() != LEX_BOOL )
        throw "expression is not boolean";
    lex_stack.pop();
}

void Parser::check_id() {
    if ( !(ID_table[c_val].get_declared()) )
        throw Exception("Parser error: use of undeclared ID: ", ID_table[c_val].get_name());
}

void Parser::check_op() {
    lex_t operand1, operand2, operation, result_type;
    operand2 = lex_stack.top();
    lex_stack.pop();
    operation = lex_stack.top();
    lex_stack.pop();
    operand1 = lex_stack.top();
    lex_stack.pop();

    if(operand1 == LEX_INT){
        if(operation == LEX_PLUS || operation == LEX_MINUS ||
                operation == LEX_TIMES || operation == LEX_SLASH || operation == LEX_ASSIGN){
            result_type = LEX_INT;
        } else if ( operation == LEX_EQ || operation == LEX_LSS || operation == LEX_GTR ||
                    operation == LEX_LEQ || operation == LEX_GEQ || operation == LEX_NEQ ){
            result_type = LEX_BOOL;
        } else throw Exception("Parser error: wrong type in operation: ", Lex::lex_map[operation]);
        if (operand1 != operand2)
            throw Exception("Parser error: wrong type in operation: ", Lex::lex_map[operation]);
    } else if(operand1 == LEX_BOOL){
        if(operation == LEX_OR || operation == LEX_AND){
            result_type = LEX_BOOL;
        } else throw Exception("Parser error: wrong type in operation: ", Lex::lex_map[operation]);
        if (operand1 != operand2)
            throw Exception("Parser error: wrong type in operation: ", Lex::lex_map[operation]);
    } else if(operand1 == LEX_STRING || operand1 == LEX_STRC){
        if(operation == LEX_PLUS){
            result_type = LEX_STRING;
        } else if(operation == LEX_EQ || operation == LEX_NEQ ){
            result_type = LEX_STRING;
        } else throw Exception("Parser error: wrong type in operation: ", Lex::lex_map[operation]);
        if (operand2 != LEX_STRING && operand2 != LEX_STRC)
            throw Exception("Parser error: wrong type in operation: ", Lex::lex_map[operation]);
    } else throw Exception("Parser error: wrong lexem: ", Lex::lex_map[operation]);
    lex_stack.push(result_type);
    prog.push_back(operation);

}

void Parser::check_not() {
    if (lex_stack.top() != LEX_BOOL)
        throw Exception("Parser error: wrong type in \"not\"");
    else
    {
        lex_stack.pop();
        lex_stack.push (LEX_BOOL);
        prog.push_back(LEX_NOT);
    }
}

void Parser::check_assign() {
    if ( lex_stack.top() != lex_stack.top() )
        throw Exception("Parser error: wrong types in assign");
    lex_stack.pop();
    lex_stack.pop();
}
