//
// Created by boris on 06.05.16.
//

#include "Int_Pair.h"

Int_Pair::Int_Pair(int value_par, int add_value_par):
        value(value_par),
        add_value(add_value_par){}

Int_Pair::Int_Pair(const Int_Pair &par):
        value(par.value),
        add_value(par.add_value){}

void Int_Pair::operator=(const Int_Pair &par) {
    value = par.value;
    add_value = par.add_value;
}
