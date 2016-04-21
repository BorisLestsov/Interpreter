//
// Created by boris on 19.04.16.
//

#include <cstring>
#include "Exception.h"

const char* Exception::what() const throw(){
    return string(err_msg).append(err_word).c_str();
}

Exception::Exception(const char *err_msg_par, const string& err_word_par): err_word(err_word_par) {
    err_msg = new char[strlen(err_msg_par) + 1];
    strcpy(err_msg, err_msg_par);
}

Exception::~Exception() {
    delete []err_msg;
}

Exception::Exception(const char *err_msg_par, const char c_par) {
    err_msg = new char[strlen(err_msg_par) + 1];
    strcpy(err_msg, err_msg_par);
    err_word += c_par;
}







