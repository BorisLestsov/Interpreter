//
// Created by boris on 19.04.16.
//

#pragma once

#include <exception>
#include <iostream>

using namespace std;

class Exception: public exception {
    char* err_msg;
    string err_word;
public:
    Exception(const char* err_msg_par, const string& err_word_par);
    Exception(const char* err_msg_par, const char c_par = '\0');
    ~Exception();

    const char* what() const throw() override;
};