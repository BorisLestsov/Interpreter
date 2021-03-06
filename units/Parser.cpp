//
// Created by Boris on 10.04.2016.
//

#include "Parser.h"

Parser::Parser(vector<Lex>& lex_vec_par, vector<ID_table_t>& ID_tables_vec_par):
        lex_vec(lex_vec_par),
        ID_tables_vec(ID_tables_vec_par)
{
    index = lex_vec.cbegin();
}

void Parser::get_lex() {
    c_lex = *index;
    c_type = c_lex.get_type();
    c_val = c_lex.get_value();
    c_add_val = c_lex.get_add_value();
    ++index;
    if (DEBUG_MODE)
        cout << c_lex << endl;
}

void Parser::unget_lex() {
    --index;
    --index;
    get_lex();
}

void Parser::make_tmp() {
    tmp_type = c_type;
    tmp_val = c_val;
    tmp_add_val = c_add_val;
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

    get_lex();
    while(c_type == LEX_STRUCT) {
        STRUCT_DECL();
        get_lex();
    }
    struct_init();

    if (DEBUG_MODE) {
        cout << "------------------------------" << endl;
        cout << "--------TABLES--------" << endl;
        int i;
        for (i = 0; i < ID_tables_vec.size(); ++i) {
            ID_tables_vec[i].print_table();
        }
        cout << "------------------------------" << endl;
        //get_lex();
    }
    while(c_type == LEX_INT || c_type == LEX_BOOL || c_type == LEX_STRING){
        DECL();
        get_lex();
    }
}

void Parser::STRUCT_DECL() {
    int cur_struct_index;

    get_lex();
    if(c_type == LEX_ID) {
        cur_struct_index = ID_tables_vec[0][c_val].get_value();
        if(ID_tables_vec[0][c_val].get_declared())
            throw Exception("Parser error: multiple declaration of structure: ",
                            ID_tables_vec[0][c_val].get_name());
        ID_tables_vec[0][c_val].set_declared(true);
    } else throw Exception("Parser error: expected ID but recieved lexem: ", Lex::lex_map[c_type]);
    get_lex();
    if(c_type != LEX_BEGIN) throw Exception("Parser error: expected { but recieved lexem: ", Lex::lex_map[c_type]);
    get_lex();
    while(c_type == LEX_INT || c_type == LEX_BOOL || c_type == LEX_STRING){
        SWITCH_ID(ID_tables_vec[cur_struct_index]);
        get_lex();
    }
    if(c_type != LEX_END) throw Exception("Parser error: unexpected lexem: ", Lex::lex_map[c_type]);
    get_lex();
    while(c_type != LEX_SEMICOLON){
        if(c_type == LEX_ID){
            ID_tables_vec[0][c_val].set_type(LEX_STRUCT);
            ID_tables_vec[0][c_val].set_value(cur_struct_index);
            ID_tables_vec[0][c_val].set_declared(true);
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
                        table[tmp_val].set_value(c_val);
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
                        table[tmp_val].set_value(1);
                    } else if(c_type == LEX_FALSE){
                        table[tmp_val].set_assigned(true);
                        table[tmp_val].set_value(0);
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
                        table[tmp_val].set_value(c_val);
                    } else throw Exception("Parser error: expected string constant but recieved lexem: ", Lex::lex_map[c_type]);
                    get_lex();
                } else {
                    table[tmp_val].set_assigned(false);
                    table.add_id(ID(LEX_STRC, "", 0));
                    table[tmp_val].set_value((int) table.get_size() - 1);
                }
                if(c_type == LEX_COMMA || c_type == LEX_SEMICOLON)
                    continue;
                else throw Exception("Parser error: unexpected lexem: ", Lex::lex_map[c_type]);
            }
            break;
        default: throw Exception("Parser error: unexpected lexem: ", Lex::lex_map[c_type]);
    }
}

void Parser::DECL(){
    SWITCH_ID(ID_tables_vec[0]);
}

void Parser::OPERATORS(){
    while(c_type != LEX_FIN && c_type != LEX_END){
        OP();
        if(c_type != LEX_SEMICOLON)
            throw Exception("Parser error: unexpected lexem: ", Lex::lex_map[c_type]);
        get_lex();
    }
    get_lex();
    if (c_type != LEX_FIN)
        throw Exception("Parser error: expected LEX_FIN but recieved lexem: ", Lex::lex_map[c_type]);
    ID_tables_vec[0].check_labels();
    prog.push_back(Lex(LEX_FIN));
    if(!break_stack.empty())
        throw Exception("Parser error: \"break\" outside of loop");
}

void Parser::OP(){
    int blank_pos0, blank_pos1, blank_pos2, blank_pos3;
    int break_pos;

    switch (c_type) {
        case LEX_IF:
            get_lex();
            EXPRESSION();
            eq_bool();
            blank_pos2 = prog.get_pos ();
            prog.blank();
            prog.push_back(Lex(RPN_FGOTO));
            OP();
            blank_pos3 = prog.get_pos();
            prog[blank_pos2] = Lex(RPN_LABEL, prog.get_pos());
            get_lex();
            if (c_type == LEX_ELSE) {
                prog.blank();
                prog.push_back(Lex(RPN_GOTO));
                get_lex();
                OP();
                prog[blank_pos3] = Lex(RPN_LABEL,prog.get_pos());
            } else { unget_lex();}
            break;
        case LEX_WHILE:
            break_stack.push(-1);

            blank_pos0 = prog.get_pos();
            get_lex();
            EXPRESSION();
            eq_bool();
            blank_pos1 = prog.get_pos();
            prog.blank();
            prog.push_back(Lex(RPN_FGOTO));
            OP();
            prog.push_back(Lex(RPN_LABEL, blank_pos0));
            prog.push_back(Lex(RPN_GOTO));
            prog[blank_pos1] = Lex(RPN_LABEL, prog.get_pos());

            break_pos = break_stack.top();
            break_stack.pop();
            while(break_pos != -1){
                prog[break_pos] = Lex(RPN_LABEL, prog.get_pos());
                break_pos = break_stack.top();
                break_stack.pop();
            }
            break;
        case LEX_FOR:
            break_stack.push(-1);

            get_lex();
            if(c_type != LEX_LPAREN)
                throw Exception("Parser error: expected \"(\": ", Lex::lex_map[c_type]);
            get_lex();
            OP();
            blank_pos0 = prog.get_pos();
            get_lex();
            EXPRESSION();
            get_lex();
            eq_bool();
            blank_pos1 = prog.get_pos();
            prog.blank();
            prog.push_back(Lex(RPN_FGOTO));
            blank_pos2 = prog.get_pos();
            prog.blank();
            prog.push_back(Lex(RPN_GOTO));
            blank_pos3 = prog.get_pos();
            OP();
            get_lex();
            prog.push_back(Lex(RPN_LABEL, blank_pos0));
            prog.push_back(Lex(RPN_GOTO));
            prog[blank_pos2] = Lex(RPN_LABEL, prog.get_pos());
            if(c_type != LEX_RPAREN)
                throw Exception("Parser error: expected \")\": ", Lex::lex_map[c_type]);
            get_lex();
            OP();
            prog.push_back(Lex(RPN_LABEL, blank_pos3));
            prog.push_back(Lex(RPN_GOTO));
            prog[blank_pos1] = Lex(RPN_LABEL, prog.get_pos());

            break_pos = break_stack.top();
            break_stack.pop();
            while(break_pos != -1){
                prog[break_pos] = Lex(RPN_LABEL, prog.get_pos());
                break_pos = break_stack.top();
                break_stack.pop();
            }
            break;
        case LEX_READ:
            get_lex();
            if (c_type == LEX_LPAREN) {
                get_lex();
                if (c_type == LEX_ID) {
                    if(ID_tables_vec[c_add_val][c_val].get_type() != LEX_INT &&
                            ID_tables_vec[c_add_val][c_val].get_type() != LEX_BOOL &&
                            ID_tables_vec[c_add_val][c_val].get_type() != LEX_STRING)
                        throw Exception("Parser error: expected int, bool or string to read, but recieved : ",
                                        ID_tables_vec[c_add_val][c_val].get_type());
                    check_id();
                    prog.push_back(Lex(RPN_ADDRESS, c_val, c_add_val));
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
                while(c_type == LEX_COMMA){
                    prog.push_back(Lex(LEX_WRITE));
                    get_lex();
                    EXPRESSION();
                }
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
                    if (!(ID_tables_vec[tmp_add_val][tmp_val].get_declared()))
                        throw Exception("Parser error: use of undeclared ID: ", ID_tables_vec[c_add_val][c_val].get_name());
                    prog.push_back(Lex(RPN_ADDRESS, tmp_val, tmp_add_val));
                    lex_stack.push(Lex(ID_tables_vec[tmp_add_val][tmp_val].get_type(),
                                       ID_tables_vec[tmp_add_val][tmp_val].get_value()));
                                //Lex:  type    ID type in tables
                                //      val     ID value in tables
                    get_lex();
                    EXPRESSION();
                    check_assign();
                    prog.push_back(Lex(LEX_ASSIGN));
                    break;
                case LEX_COLON:
                    if(ID_tables_vec[0][tmp_val].get_type() == LEX_LABEL) {
                        //found previously in goto
                        if (ID_tables_vec[0][tmp_val].get_assigned())
                            throw Exception("Parser error: multiple assign of Label: ",
                                            ID_tables_vec[0][tmp_val].get_name());
                        prog[ID_tables_vec[0][tmp_val].get_value()] = Lex(RPN_LABEL, prog.get_pos());
                        ID_tables_vec[0][tmp_val].set_value(prog.get_pos() - 1);
                        ID_tables_vec[0][tmp_val].set_assigned(true);
                    } else {
                        tmp_type = ID_tables_vec[0][tmp_val].get_type();
                        if (tmp_type == LEX_INT || tmp_type == LEX_BOOL || tmp_type == LEX_STRING ||
                            tmp_type == LEX_STRC)
                            throw Exception("Parser error: attempt to use previously declared ID as Label: ",
                                            ID_tables_vec[0][tmp_val].get_name());
                        ID_tables_vec[0][tmp_val].set_type(LEX_LABEL);
                        if (ID_tables_vec[0].multiple_declaration(tmp_val))
                            throw Exception("Parser error: multiple declaration of Label: ",
                                            ID_tables_vec[0][tmp_val].get_name());
                        ID_tables_vec[0][tmp_val].set_value(prog.get_pos());
                        ID_tables_vec[0][tmp_val].set_declared(true);
                        ID_tables_vec[0][tmp_val].set_assigned(true);
                    }
                    c_type = LEX_SEMICOLON;
                    get_lex();
                    OP();
                    break;
                default:
                    throw Exception("Parser error: unexpected lexem: ", Lex::lex_map[c_type]);
            }
            break;
        case LEX_BEGIN:
            COMPLEX_OP();
            break;
        case LEX_SEMICOLON:
            //get_lex();
            break;
        case LEX_BREAK:
            break_stack.push(prog.get_pos());
            prog.blank();
            prog.push_back(Lex(RPN_GOTO));
            get_lex();
            break;
        case LEX_GOTO:
            get_lex();
            if(c_type != LEX_ID)
                throw Exception("Parser error: expected Label: ", Lex::lex_map[c_type]);
            if(ID_tables_vec[0][c_val].get_type() == LEX_LABEL){
                prog.push_back(Lex(RPN_LABEL, ID_tables_vec[0][c_val].get_value()));
                prog.push_back(Lex(RPN_GOTO));
            } else {
                tmp_type = ID_tables_vec[0][c_val].get_type();
                if(tmp_type == LEX_INT || tmp_type == LEX_BOOL || tmp_type == LEX_STRING ||
                        tmp_type == LEX_STRC || tmp_type == LEX_STRUCT_T || tmp_type == LEX_STRUCT)
                    throw Exception("Parser error: attempt to use previously declared ID as Label: ",
                                    ID_tables_vec[0][c_val].get_name());
                ID_tables_vec[0][c_val].set_type(LEX_LABEL);
                ID_tables_vec[0][c_val].set_declared(true);
                ID_tables_vec[0][c_val].set_assigned(false);
                ID_tables_vec[0][c_val].set_value(prog.get_pos());
                prog.blank();
                prog.push_back(Lex(RPN_GOTO));
            }
            get_lex();
            break;
        default:
            throw Exception("Parser error: Unknown operator: ", Lex::lex_map[c_type]);
    }
}

void Parser::COMPLEX_OP() {
    if (c_type == LEX_BEGIN) {
        do {
            get_lex();
            if (c_type == LEX_END) break;
            OP();
        } while (c_type == LEX_SEMICOLON);
        if (c_type == LEX_END)
            c_type = LEX_SEMICOLON;
        else
            throw Exception("Parser error: expected \"}\" but recieved: ", Lex::lex_map[c_type]);
    }
    else throw Exception("Parser error: expected \"{\" but recieved: ", Lex::lex_map[c_type]);
}
void Parser::EXPRESSION() {
    LOW();
    if (c_type == LEX_OR)
    {
        lex_stack.push (c_type);
        get_lex();
        LOW();
        check_op();
    }
}

void Parser::LOW(){
    MID();
    if(c_type == LEX_AND)
    {
        lex_stack.push (c_type);
        get_lex();
        MID();
        check_op();
    }
}

void Parser::MID(){
    HIGH();
    if (c_type == LEX_EQ || c_type == LEX_LSS || c_type == LEX_GTR ||
         c_type == LEX_LEQ || c_type == LEX_GEQ || c_type == LEX_NEQ)
    {
        lex_stack.push (c_type);
        get_lex();
        HIGH();
        check_op();
    }
}

void Parser::HIGH() {
    ULTRA_HIGH();
    while ( c_type == LEX_PLUS || c_type == LEX_MINUS ) {
        lex_stack.push(c_type);
        get_lex();
        ULTRA_HIGH();
        check_op();
    }
}

void Parser::ULTRA_HIGH() {
    ULTRA_ULTRA_HIGH();
    while ( c_type == LEX_TIMES || c_type == LEX_SLASH) {
        lex_stack.push (c_type);
        get_lex();
        ULTRA_ULTRA_HIGH();
        check_op();
    }
}
void Parser::ULTRA_ULTRA_HIGH() {
    if (c_type == LEX_PLUS) {
        lex_stack.push(LEX_UPLUS);
        get_lex();
        F();
        check_left_op();
    } else if (c_type == LEX_MINUS) {
        lex_stack.push(LEX_UMINUS);
        get_lex();
        F();
        check_left_op();
    } else F();
}
void Parser::check_left_op() {
    lex_t operand, operation;
    operand = lex_stack.top().get_type();
    lex_stack.pop();
    operation = lex_stack.top().get_type();
    lex_stack.pop();
    if(operation != LEX_UPLUS && operation != LEX_UMINUS)
        throw Exception("Parser error: unary operator: ", Lex::lex_map[operation]);
    if(operand != LEX_INT)
        throw Exception("Parser error: unary operator to: ", Lex::lex_map[operand]);
    lex_stack.push(LEX_INT);
    prog.push_back(operation);
}


void Parser::F ()
{
    switch (c_type){
        case LEX_ID:
            check_id();
            lex_stack.push(Lex(ID_tables_vec[c_add_val][c_val].get_type(),
                           ID_tables_vec[c_add_val][c_val].get_value()));
            prog.push_back ( c_lex );
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
                throw Exception("Parser error: expected \")\" but recieved: ", Lex::lex_map[c_type]);
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
    if ( lex_stack.top().get_type() != LEX_BOOL )
        throw "expression is not boolean";
    lex_stack.pop();
}

void Parser::check_id() {
    if ( !(ID_tables_vec[c_add_val][c_val].get_declared()) )
        throw Exception("Parser error: use of undeclared ID: ", ID_tables_vec[c_add_val][c_val].get_name());
}

void Parser::check_op() {
    lex_t operand1, operand2, operation, result_type;
    operand2 = lex_stack.top().get_type();
    lex_stack.pop();
    operation = lex_stack.top().get_type();
    lex_stack.pop();
    operand1 = lex_stack.top().get_type();
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
        } else if(operation == LEX_EQ || operation == LEX_LSS || operation == LEX_GTR ||
                  operation == LEX_LEQ || operation == LEX_GEQ || operation == LEX_NEQ){
            result_type = LEX_BOOL;
        } else throw Exception("Parser error: wrong type in operation: ", Lex::lex_map[operation]);
        if (operand2 != LEX_STRING && operand2 != LEX_STRC)
            throw Exception("Parser error: wrong type in operation: ", Lex::lex_map[operation]);
    } else throw Exception("Parser error: wrong operation: ", Lex::lex_map[operation]);
    lex_stack.push(result_type);
    prog.push_back(operation);

}

void Parser::check_not() {
    if (lex_stack.top().get_type() != LEX_BOOL)
        throw Exception("Parser error: wrong type in \"not\"");
    else
    {
        lex_stack.pop();
        lex_stack.push (LEX_BOOL);
        prog.push_back(LEX_NOT);
    }
}

void Parser::check_assign() {
    Lex op1 = lex_stack.top();
    lex_stack.pop();
    Lex op2 = lex_stack.top();
    lex_stack.pop();
    if(op2.get_type() != LEX_STRING){
        if ( op1.get_type() != op2.get_type() )
            throw Exception("Parser error: wrong types in assign");
    } else if(op1.get_type() != LEX_STRING && op1.get_type() != LEX_STRC)
        throw Exception("Parser error: wrong types in assign");
    if(op1.get_type() == LEX_STRUCT_T)
        throw Exception("Parser error: structure type in assign");
    if(op1.get_type() == LEX_STRUCT)
        if(ID_tables_vec[op1.get_value()][0].get_value() != ID_tables_vec[op2.get_value()][0].get_value())
            throw Exception("Parser error: assignment of structures of diffrent types");

}

void Parser::struct_init() {
    auto ptr = ID_tables_vec[0].get_begin();
    auto end = ID_tables_vec[0].get_end();
    int table_index;

    while(ptr != end){
        if(ptr->get_type() == LEX_STRUCT){
            table_index = (int) ID_tables_vec.size();
            ID_tables_vec.push_back(ID_tables_vec[ptr->get_value()]);
            ptr->set_value(table_index);
        }
        ++ptr;
    }

    if (DEBUG_MODE)
        cout << endl << endl;
    auto lex_ptr = lex_vec.begin();
    auto lex_end = lex_vec.end();
    while(lex_ptr != lex_end){
        if(lex_ptr->get_add_value() != 0){
            lex_ptr->set_add_value(ID_tables_vec[0][lex_ptr->get_add_value()].get_value());
        }
        ++lex_ptr;
    }

}

