#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <chrono>

using namespace std;

#include "math/equation.cpp"



int main()
{
    equation e(4, -10, 35, -50, 24);
    for(unsigned int i=0; i<e.length(); i++)
        cout<<e[i]<<endl;
}