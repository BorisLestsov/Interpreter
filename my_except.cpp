//
// Created by boris on 19.04.16.
//

#include "my_except.h"

const char* my_except::what() const throw(){
    return "error: ...";
}