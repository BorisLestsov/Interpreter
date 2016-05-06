//
// Created by boris on 06.05.16.
//

#pragma once

struct Int_Pair{
    int value;
    int add_value;

    Int_Pair(int value_par, int add_value_par = 0);
    Int_Pair(const Int_Pair& par);
    void operator=(const Int_Pair& par);
};