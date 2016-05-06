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
    arg1_lex(LEX_NULL),
    arg2_lex(LEX_NULL){}

void Virtual_Machine::get_cmd() {
    static int i = 0;

    c_cmd = *cmd_ptr;
    ++cmd_ptr;

    //cout << setw(2) << i++  << c_cmd << endl;
}

bool Virtual_Machine::prog_finished() const {
    return cmd_ptr == prog.cend();
}

void Virtual_Machine::make_c_lex() {
    c_lex = c_cmd;
}

void Virtual_Machine::make_tmp() {
    arg1_lex = c_lex;
}

int Virtual_Machine::start() {
    stack<Lex> args;

    while(!prog_finished()){
        get_cmd();
        switch(c_cmd.get_type()){
            case LEX_NUM:
            case LEX_TRUE:
            case LEX_FALSE:
            case RPN_ADDRESS:
            case RPN_LABEL:
            case LEX_STRC:
            case LEX_ID:
                make_c_lex();
                args.push(c_lex);
                break;
            case LEX_ASSIGN:
                arg2_lex = args.top();
                args.pop();
                arg1_lex = args.top();
                args.pop();
                switch (ID_tables_vec[arg1_lex.get_add_value()][arg1_lex.get_value()].get_type()) {
                    case LEX_INT:
                        if(arg2_lex.get_type() == LEX_NUM)
                            tmp1 = arg2_lex.get_value();
                        else if(arg2_lex.get_type() == LEX_ID){
                            tmp1 = ID_tables_vec[arg2_lex.get_add_value()][arg2_lex.get_value()].get_value();
                        } else throw Exception("Runtime error: assignment error");
                        ID_tables_vec[arg1_lex.get_add_value()][arg1_lex.get_value()].set_value(tmp1);
                        break;
                    case LEX_BOOL:
                        if(arg2_lex.get_type() == LEX_BOOL)
                            tmp1 = arg2_lex.get_value();
                        else if(arg2_lex.get_type() == LEX_ID){
                            tmp1 = ID_tables_vec[arg2_lex.get_add_value()][arg2_lex.get_value()].get_value();
                        } else throw Exception("Runtime error: assignment error");
                        ID_tables_vec[arg1_lex.get_add_value()][arg1_lex.get_value()].set_value(tmp1);
                        break;
                    case LEX_STRING:
                        if(arg2_lex.get_type() == LEX_STRC)
                            tmp_str = ID_tables_vec[arg2_lex.get_add_value()][arg2_lex.get_value()].get_name();
                        else if(arg2_lex.get_type() == LEX_STRING){
                            tmp1 = ID_tables_vec[arg2_lex.get_add_value()][arg2_lex.get_value()].get_value();
                            tmp_str = ID_tables_vec[arg2_lex.get_add_value()][tmp1].get_name();
                        } else throw Exception("Runtime error: assignment error");
                        ID_tables_vec[arg1_lex.get_add_value()][ID_tables_vec[arg1_lex.get_add_value()]
                        [arg1_lex.get_value()].get_value()].set_name(tmp_str);
                        break;
                    case LEX_STRUCT:
                        break;

                }
                break;
            case LEX_PLUS:
                lex_t res_t;

                arg2_lex = args.top();
                args.pop();
                arg1_lex = args.top();
                args.pop();
                if(arg1_lex.get_type() == LEX_NUM){
                    tmp1 = arg1_lex.get_value();
                    res_t = LEX_NUM;
                } else if(arg1_lex.get_type() == LEX_ID) {
                    if (ID_tables_vec[arg1_lex.get_add_value()][arg1_lex.get_value()].get_type() ==
                        LEX_INT){
                        tmp1 = ID_tables_vec[arg1_lex.get_add_value()][arg1_lex.get_value()].get_value();
                        res_t = LEX_NUM;
                    } else if(ID_tables_vec[arg1_lex.get_add_value()][arg1_lex.get_value()].get_type() ==
                              LEX_STRING) {
                        tmp_str = ID_tables_vec[arg1_lex.get_add_value()]
                        [ID_tables_vec[arg1_lex.get_add_value()][arg1_lex.get_value()].get_value()].get_name();
                        res_t = LEX_STRC;
                    }
                } else if(arg1_lex.get_type() == LEX_STRC){
                    if(arg1_lex.get_value() >= 0)
                        tmp_str = ID_tables_vec[arg1_lex.get_add_value()][arg1_lex.get_value()].get_name();
                    res_t = LEX_STRC;
                } else throw Exception("Runtime error: error in operator +");

                if(res_t == LEX_NUM){
                    if(arg1_lex.get_type() == LEX_NUM)
                        tmp2 = arg2_lex.get_value();
                    else
                        tmp2 = ID_tables_vec[arg2_lex.get_add_value()][arg2_lex.get_value()].get_value();
                    args.push(Lex(LEX_NUM, tmp1 + tmp2));
                } else {
                    if(arg2_lex.get_type() == LEX_STRC){
                        tmp_str += ID_tables_vec[arg2_lex.get_add_value()][arg2_lex.get_value()].get_name();
                    } else if(ID_tables_vec[arg2_lex.get_add_value()][arg2_lex.get_value()].get_type() ==
                              LEX_STRING) {
                        tmp_str += ID_tables_vec[arg2_lex.get_add_value()]
                        [ID_tables_vec[arg2_lex.get_add_value()][arg2_lex.get_value()].get_value()].get_name();
                    }
                    res_t = LEX_STRC;
                    args.push(Lex(LEX_STRC, -1));
                }
                break;
            case LEX_TIMES:
                arg1_lex = args.top();
                args.pop();
                arg2_lex = args.top();
                args.pop();
                if(arg1_lex.get_type() == LEX_NUM){
                    tmp1  = arg1_lex.get_value();
                } else if(arg1_lex.get_type() == LEX_ID){
                    tmp1 = ID_tables_vec[arg1_lex.get_add_value()][arg1_lex.get_value()].get_value();
                }
                if(arg2_lex.get_type() == LEX_NUM){
                    tmp2  = arg2_lex.get_value();
                } else if(arg2_lex.get_type() == LEX_ID){
                    tmp2 = ID_tables_vec[arg2_lex.get_add_value()][arg2_lex.get_value()].get_value();
                }
                args.push(Lex(LEX_NUM, tmp1 * tmp2));
                break;
            case LEX_MINUS:
                arg2_lex = args.top(); //tmp_pairs are reversed because of RPN
                args.pop();
                arg1_lex = args.top();
                args.pop();
                if(arg1_lex.get_type() == LEX_NUM){
                    tmp1  = arg1_lex.get_value();
                } else if(arg1_lex.get_type() == LEX_ID){
                    tmp1 = ID_tables_vec[arg1_lex.get_add_value()][arg1_lex.get_value()].get_value();
                }
                if(arg2_lex.get_type() == LEX_NUM){
                    tmp2  = arg2_lex.get_value();
                } else if(arg2_lex.get_type() == LEX_ID){
                    tmp2 = ID_tables_vec[arg2_lex.get_add_value()][arg2_lex.get_value()].get_value();
                }
                args.push(Lex(LEX_NUM, tmp1 - tmp2));
                break;
            case LEX_SLASH:
                arg2_lex = args.top(); //tmp_pairs are reversed because of RPN
                args.pop();
                arg1_lex = args.top();
                args.pop();
                if(arg1_lex.get_type() == LEX_NUM){
                    tmp1  = arg1_lex.get_value();
                } else if(arg1_lex.get_type() == LEX_ID){
                    tmp1 = ID_tables_vec[arg1_lex.get_add_value()][arg1_lex.get_value()].get_value();
                }
                if(arg2_lex.get_type() == LEX_NUM){
                    tmp2  = arg2_lex.get_value();
                } else if(arg2_lex.get_type() == LEX_ID){
                    tmp2 = ID_tables_vec[arg2_lex.get_add_value()][arg2_lex.get_value()].get_value();
                }
                if (tmp2)
                    args.push(Lex(LEX_NUM, tmp2 / tmp1));
                else throw Exception("Runtime error: division by zero");
                break;
            case LEX_READ:
                arg1_lex = args.top();
                args.pop();
                switch (ID_tables_vec[arg1_lex.get_add_value()][arg1_lex.get_value()].get_type()) {
                    case LEX_INT:
                        cout << "Input integer for \""
                                << ID_tables_vec[arg1_lex.get_add_value()][arg1_lex.get_value()].get_name()
                                << "\": " ;
                        cin >> tmp1;
                        ID_tables_vec[arg1_lex.get_add_value()][arg1_lex.get_value()].set_value(tmp1);
                        break;
                    case LEX_BOOL:
                        cout << "Input boolean integer for \""
                        << ID_tables_vec[arg1_lex.get_add_value()][arg1_lex.get_value()].get_name()
                        << "\": " ;
                        cin >> tmp1;
                        ID_tables_vec[arg1_lex.get_add_value()][arg1_lex.get_value()].set_value(tmp1 != 0);
                        break;
                    case LEX_STRING:
                        cout << "Input string for \""
                        << ID_tables_vec[arg1_lex.get_add_value()][arg1_lex.get_value()].get_name()
                        << "\": ";
                        cin >> tmp_str;
                        ID_tables_vec[arg1_lex.get_add_value()][ID_tables_vec[arg1_lex.get_add_value()]
                        [arg1_lex.get_value()].get_value()].set_name(tmp_str);
                        break;
                }
                break;
            case LEX_WRITE:
                arg1_lex = args.top();
                args.pop();
                if(arg1_lex.get_type() == LEX_NUM ||
                        arg1_lex.get_type() == LEX_TRUE ||
                        arg1_lex.get_type() == LEX_FALSE)
                    cout << arg1_lex.get_value();
                else if(arg1_lex.get_type() == LEX_STRC) {
                    if(arg1_lex.get_value() >= 0)
                        cout << ID_tables_vec[arg1_lex.get_add_value()][arg1_lex.get_value()].get_name();
                    else
                        cout << tmp_str;
                } else if(arg1_lex.get_type() == LEX_ID)
                    if(ID_tables_vec[arg1_lex.get_add_value()][arg1_lex.get_value()].get_type() ==
                       LEX_INT){
                        cout << ID_tables_vec[arg1_lex.get_add_value()][arg1_lex.get_value()].get_value();
                    } else if(arg1_lex.get_type() == LEX_STRC) {
                        if (arg1_lex.get_type() >= 0)
                            cout << ID_tables_vec[arg1_lex.get_add_value()][arg1_lex.get_value()].get_name();
                        else
                            cout << tmp_str;
                    } else if(ID_tables_vec[arg1_lex.get_add_value()][arg1_lex.get_value()].get_type() ==
                              LEX_STRING)
                        cout << ID_tables_vec[arg1_lex.get_add_value()]
                        [ID_tables_vec[arg1_lex.get_add_value()][arg1_lex.get_value()].get_value()].get_name();
                break;
            case LEX_FIN:
                cout << "$$$$$$ Program finished correctly $$$$$$" << endl;
                break;
            default: throw Exception("Runtime error: unexpected command: ", Lex::lex_map[c_cmd.get_type()]);
        }

    }
    return 0;
}

