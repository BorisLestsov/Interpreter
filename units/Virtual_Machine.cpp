//
// Created by boris on 05.05.16.
//

#include "Virtual_Machine.h"
#include <stack>
#include "Exception.h"

Virtual_Machine::Virtual_Machine(RPN &prog_par, vector<ID_table_t>& ID_tables_vec_par):
    prog(prog_par),
    cmd_ptr(prog.cbegin()),
    ID_tables_vec(ID_tables_vec_par),
    c_lex(LEX_NULL),
    arg1(LEX_NULL),
    arg2(LEX_NULL){}

void Virtual_Machine::get_cmd() {
    static int i = 0;

    c_cmd = *cmd_ptr;
    ++cmd_ptr;

    if (DEBUG_MODE)
        cout << setw(2) << i++  << c_cmd << endl;
}

bool Virtual_Machine::prog_finished() const {
    return cmd_ptr == prog.cend();
}

int Virtual_Machine::start() {
    stack<Lex> args;

    while(!prog_finished()){
        get_cmd();
        switch(c_cmd.get_type()){
            case LEX_NUM:
            case RPN_ADDRESS:
            case RPN_LABEL:
                args.push(c_cmd);
                break;
            case LEX_TRUE:
                args.push(Lex(LEX_BOOL, 1));
                break;
            case LEX_FALSE:
                args.push(Lex(LEX_BOOL, 0));
                break;
            case LEX_STRC:
                args.push(c_cmd);
                tmp_str += ID_tables_vec[c_cmd.get_add_value()][c_cmd.get_value()].get_name();
                break;
            case LEX_ID:
                switch(ID_tables_vec[c_cmd.get_add_value()][c_cmd.get_value()].get_type()){
                    case LEX_INT:
                        args.push(Lex(LEX_NUM, ID_tables_vec[c_cmd.get_add_value()][c_cmd.get_value()].get_value()));
                        break;
                    case LEX_BOOL:
                        args.push(Lex(LEX_BOOL, ID_tables_vec[c_cmd.get_add_value()][c_cmd.get_value()].get_value()));
                        break;
                    case LEX_STRING:
                        args.push(Lex(LEX_STRC, ID_tables_vec[c_cmd.get_add_value()][c_cmd.get_value()].get_value(),
                                      c_cmd.get_add_value()));
                        //args.push(Lex(LEX_STRC, -1, 0));
                        tmp_str += ID_tables_vec[c_cmd.get_add_value()]
                            [ID_tables_vec[c_cmd.get_add_value()][c_cmd.get_value()].get_value()].get_name();
                        break;
                    case LEX_STRC:
                        args.push(Lex(LEX_STRC, c_cmd.get_value(), c_cmd.get_add_value()));
                        //args.push(Lex(LEX_STRC, -1, 0));
                        tmp_str += ID_tables_vec[c_cmd.get_add_value()][c_cmd.get_value()].get_name();
                        break;
                    case LEX_STRUCT:
                        args.push(Lex(LEX_STRUCT, c_cmd.get_value()));
                        break;
                    default: throw Exception("Runtime error: unknown ID");
                }
                break;
            case LEX_ASSIGN:
                arg2 = args.top();
                args.pop();
                arg1 = args.top();
                args.pop();
                switch (ID_tables_vec[arg1.get_add_value()][arg1.get_value()].get_type()) {
                    case LEX_INT:
                        if(arg2.get_type() == LEX_NUM)
                            tmp1 = arg2.get_value();
                        else throw Exception("Runtime error: assignment error");
                        ID_tables_vec[arg1.get_add_value()][arg1.get_value()].set_value(tmp1);
                        break;
                    case LEX_BOOL:
                        if(arg2.get_type() == LEX_BOOL)
                            tmp1 = arg2.get_value();
                        else throw Exception("Runtime error: assignment error");
                        ID_tables_vec[arg1.get_add_value()][arg1.get_value()].set_value(tmp1);
                        break;
                    case LEX_STRING:
                        if(arg2.get_type() != LEX_STRC)
                            throw Exception("Runtime error: assignment error");
                        ID_tables_vec[arg1.get_add_value()][ID_tables_vec[arg1.get_add_value()]
                        [arg1.get_value()].get_value()].set_name(tmp_str);
                        tmp_str.clear();
                        break;
                    case LEX_STRUCT:
                        ID_tables_vec[ID_tables_vec[0][arg1.get_value()].get_value()] =
                                ID_tables_vec[ID_tables_vec[0][arg2.get_value()].get_value()];
                        break;
                    default: throw Exception("Runtime error: assignment error");
                }
                break;
            case LEX_PLUS:
                lex_t res_t;

                arg2 = args.top();
                args.pop();
                arg1 = args.top();
                args.pop();
                if(arg1.get_type() == LEX_NUM){
                    tmp1 = arg1.get_value();
                    res_t = LEX_NUM;
                } else if(arg1.get_type() == LEX_STRC){
                    if(arg1.get_value() >= 0){
                        if(arg2.get_value() >=0 )
                            tmp_str = ID_tables_vec[arg1.get_add_value()][arg1.get_value()].get_name();
                        else tmp_str.insert(0, ID_tables_vec[arg1.get_add_value()][arg1.get_value()].get_name());
                    }
                    res_t = LEX_STRC;
                } else throw Exception("Runtime error: error in operator +");

                if(res_t == LEX_NUM){
                    if(arg1.get_type() == LEX_NUM)
                        tmp2 = arg2.get_value();
                    else
                        tmp2 = ID_tables_vec[arg2.get_add_value()][arg2.get_value()].get_value();
                    args.push(Lex(LEX_NUM, tmp1 + tmp2));
                } else {
                    if (arg2.get_type() == LEX_STRC)
                        if(arg2.get_value() >= 0)
                            tmp_str += ID_tables_vec[arg2.get_add_value()][arg2.get_value()].get_name();
                        res_t = LEX_STRC;
                        args.push(Lex(LEX_STRC, -1));
                }
                break;
            case LEX_TIMES:
                arg1 = args.top();
                args.pop();
                arg2 = args.top();
                args.pop();
                if(arg1.get_type() == LEX_NUM){
                    tmp1  = arg1.get_value();
                }
                if(arg2.get_type() == LEX_NUM){
                    tmp2  = arg2.get_value();
                }
                args.push(Lex(LEX_NUM, tmp1 * tmp2));
                break;
            case LEX_MINUS:
                arg2 = args.top(); //tmp_pairs are reversed because of RPN
                args.pop();
                arg1 = args.top();
                args.pop();
                if(arg1.get_type() == LEX_NUM){
                    tmp1  = arg1.get_value();
                }
                if(arg2.get_type() == LEX_NUM){
                    tmp2  = arg2.get_value();
                }
                args.push(Lex(LEX_NUM, tmp1 - tmp2));
                break;
            case LEX_UPLUS:
                arg1 = args.top();
                args.pop();
                if(arg1.get_type() == LEX_NUM)
                    args.push(Lex(LEX_NUM, arg1.get_value()));
                else throw Exception("Runtime error: \"unary +\" to non-integer expression");
                break;
            case LEX_UMINUS:
                arg1 = args.top();
                args.pop();
                if(arg1.get_type() == LEX_NUM)
                    args.push(Lex(LEX_NUM, -arg1.get_value()));
                else throw Exception("Runtime error: \"unary -\" to non-integer expression");
                break;
            case LEX_SLASH:
                arg2 = args.top(); //tmp_pairs are reversed because of RPN
                args.pop();
                arg1 = args.top();
                args.pop();
                if(arg1.get_type() == LEX_NUM){
                    tmp1  = arg1.get_value();
                }
                if(arg2.get_type() == LEX_NUM){
                    tmp2  = arg2.get_value();
                }
                if (tmp2)
                    args.push(Lex(LEX_NUM, tmp1 / tmp2));
                else throw Exception("Runtime error: division by zero");
                break;
            case LEX_READ:
                arg1 = args.top();
                args.pop();
                switch (ID_tables_vec[arg1.get_add_value()][arg1.get_value()].get_type()) {
                    case LEX_INT:
                        cout << "Input integer for \""
                                << ID_tables_vec[arg1.get_add_value()][arg1.get_value()].get_name()
                                << "\": " ;
                        cin >> tmp1;
                        ID_tables_vec[arg1.get_add_value()][arg1.get_value()].set_value(tmp1);
                        break;
                    case LEX_BOOL:
                        cout << "Input boolean integer for \""
                        << ID_tables_vec[arg1.get_add_value()][arg1.get_value()].get_name()
                        << "\": " ;
                        cin >> tmp1;
                        ID_tables_vec[arg1.get_add_value()][arg1.get_value()].set_value(tmp1 != 0);
                        break;
                    case LEX_STRING:
                        cout << "Input string for \""
                        << ID_tables_vec[arg1.get_add_value()][arg1.get_value()].get_name()
                        << "\": ";
                        cin >> tmp_str;
                        ID_tables_vec[arg1.get_add_value()][ID_tables_vec[arg1.get_add_value()]
                        [arg1.get_value()].get_value()].set_name(tmp_str);
                        break;
                }
                break;
            case LEX_WRITE:
                arg1 = args.top();
                args.pop();
                if(arg1.get_type() == LEX_NUM ||
                   arg1.get_type() == LEX_BOOL)
                    cout << arg1.get_value();
                else if(arg1.get_type() == LEX_STRC) {
                    if(arg1.get_value() >= 0)
                        cout << ID_tables_vec[arg1.get_add_value()][arg1.get_value()].get_name();
                    else
                        cout << tmp_str;
                }
                tmp_str.clear();
                break;
            case LEX_NOT:
                arg1 = args.top();
                args.pop();
                if(arg1.get_type() == LEX_BOOL)
                    args.push(Lex(LEX_BOOL, arg1.get_value()));
                else throw Exception("Runtime error: \"not\" in non-boolean expression");
                break;
            case LEX_OR:
                arg2 = args.top();
                args.pop();
                arg1 = args.top();
                args.pop();
                if(arg1.get_type() == LEX_BOOL && arg2.get_type() == LEX_BOOL)
                    args.push(Lex(LEX_BOOL, arg1.get_value() || arg2.get_value()));
                else throw Exception("Runtime error: \"or\" in non-boolean expression");
                break;
            case LEX_AND:
                arg2 = args.top();
                args.pop();
                arg1 = args.top();
                args.pop();
                if(arg1.get_type() == LEX_BOOL && arg2.get_type() == LEX_BOOL)
                    args.push(Lex(LEX_BOOL, arg1.get_value() && arg2.get_value()));
                else throw Exception("Runtime error: \"or\" in non-boolean expression");
                break;
            case LEX_EQ:
                arg2 = args.top();
                args.pop();
                arg1 = args.top();
                args.pop();
                if(arg1.get_type() != arg2.get_type())
                    throw Exception("Runtime error: == comparison of wrong types");
                if(arg1.get_type() == LEX_STRC)
                    args.push(Lex(LEX_BOOL, ID_tables_vec[arg1.get_add_value()][arg1.get_value()].get_name() ==
                            ID_tables_vec[arg2.get_add_value()][arg2.get_value()].get_name()));
                else
                    args.push(Lex(LEX_BOOL, arg1.get_value() == arg2.get_value()));
                break;
            case LEX_LSS:
                arg2 = args.top();
                args.pop();
                arg1 = args.top();
                args.pop();
                if(arg1.get_type() != arg2.get_type())
                    throw Exception("Runtime error: < comparison of wrong types");
                if(arg1.get_type() == LEX_STRC)
                    args.push(Lex(LEX_BOOL, ID_tables_vec[arg1.get_add_value()][arg1.get_value()].get_name() <
                                            ID_tables_vec[arg2.get_add_value()][arg2.get_value()].get_name()));
                else
                    args.push(Lex(LEX_BOOL, arg1.get_value() < arg2.get_value()));
                break;
            case LEX_GTR:
                arg2 = args.top();
                args.pop();
                arg1 = args.top();
                args.pop();
                if(arg1.get_type() != arg2.get_type())
                    throw Exception("Runtime error: > comparison of wrong types");
                if(arg1.get_type() == LEX_STRC)
                    args.push(Lex(LEX_BOOL, ID_tables_vec[arg1.get_add_value()][arg1.get_value()].get_name() >
                                            ID_tables_vec[arg2.get_add_value()][arg2.get_value()].get_name()));
                else
                    args.push(Lex(LEX_BOOL, arg1.get_value() > arg2.get_value()));
                break;
            case LEX_LEQ:
                arg2 = args.top();
                args.pop();
                arg1 = args.top();
                args.pop();
                if(arg1.get_type() != arg2.get_type())
                    throw Exception("Runtime error: <= comparison of wrong types");
                if(arg1.get_type() == LEX_STRC)
                    args.push(Lex(LEX_BOOL, ID_tables_vec[arg1.get_add_value()][arg1.get_value()].get_name() <=
                                            ID_tables_vec[arg2.get_add_value()][arg2.get_value()].get_name()));
                else
                    args.push(Lex(LEX_BOOL, arg1.get_value() <= arg2.get_value()));
                break;
            case LEX_GEQ:
                arg2 = args.top();
                args.pop();
                arg1 = args.top();
                args.pop();
                if(arg1.get_type() != arg2.get_type())
                    throw Exception("Runtime error: >= comparison of wrong types");
                if(arg1.get_type() == LEX_STRC)
                    args.push(Lex(LEX_BOOL, ID_tables_vec[arg1.get_add_value()][arg1.get_value()].get_name() >=
                                            ID_tables_vec[arg2.get_add_value()][arg2.get_value()].get_name()));
                else
                    args.push(Lex(LEX_BOOL, arg1.get_value() >= arg2.get_value()));
                break;
            case LEX_NEQ:
                arg2 = args.top();
                args.pop();
                arg1 = args.top();
                args.pop();
                if(arg1.get_type() != arg2.get_type())
                    throw Exception("Runtime error: != comparison of wrong types");
                if(arg1.get_type() == LEX_STRC)
                    args.push(Lex(LEX_BOOL, ID_tables_vec[arg1.get_add_value()][arg1.get_value()].get_name() !=
                                            ID_tables_vec[arg2.get_add_value()][arg2.get_value()].get_name()));
                else
                    args.push(Lex(LEX_BOOL, arg1.get_value() != arg2.get_value()));
                break;
            case RPN_GOTO:
                arg1 = args.top();
                args.pop();
                if(arg1.get_type() != RPN_LABEL)
                    throw Exception("Runtime error: expected label in RPN_GOTO, but recieved: ", Lex::lex_map[c_cmd.get_type()]);
                cmd_ptr = prog.begin() + arg1.get_value();
                break;
            case RPN_FGOTO:
                arg2 = args.top();
                args.pop();

                arg1 = args.top();
                args.pop();
                if(arg2.get_type() != RPN_LABEL)
                    throw Exception("Runtime error: expected label in RPN_FGOTO, but recieved: ",
                                    Lex::lex_map[c_cmd.get_type()]);
                if (arg1.get_type() != LEX_BOOL)
                    throw Exception("Runtime error: expected boolean expression in RPN_FGOTO , but recieved: ",
                                    Lex::lex_map[c_cmd.get_type()]);
                if(!arg1.get_value())
                    cmd_ptr = prog.begin() + arg2.get_value();
                break;
            case LEX_FIN:
                if (DEBUG_MODE)
                    cout << "$$$$$$ Program finished correctly $$$$$$" << endl;
                break;
            default: throw Exception("Runtime error: unexpected command: ", Lex::lex_map[c_cmd.get_type()]);
        }

    }
    return 0;
}

