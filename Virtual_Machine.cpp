//
// Created by boris on 05.05.16.
//

#include "Virtual_Machine.h"
#include <stack>

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

    cout << setw(2) << i++  << c_cmd << endl;
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
            case LEX_PLUS:
                int tmp1, tmp2;
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
                }

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
            /*case LEX_TIMES:
                arg1_lex = args.top();
                args.pop();
                arg2_lex = args.top();
                args.pop();
                args.push(arg1_lex.value * arg2_lex.value);
                break;
            case LEX_MINUS:
                arg1_lex = args.top();
                args.pop();
                arg2_lex = args.top();
                args.pop();
                args.push(arg2_lex.value - arg1_lex.value);   //tmp_pairs are reversed because of RPN
                break;
            case LEX_SLASH:
                arg1_lex = args.top();
                args.pop();
                arg2_lex = args.top();
                args.pop();
                if (!arg1_lex.value)
                    args.push(arg2_lex.value - arg1_lex.value);   //tmp_pairs are reversed because of RPN
                else throw Exception("Runtime error: division by zero");
                break;*/
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
        }

    }
    return 0;
}
