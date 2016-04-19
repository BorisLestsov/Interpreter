//
// Created by boris on 19.04.16.
//

#pragma once

#include <exception>
#include <iostream>

using namespace std;

class my_except: public exception {
public:
    const char* what() const throw();
};
