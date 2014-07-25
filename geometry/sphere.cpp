#ifndef SPHERE_CPP
#define SPHERE_CPP

#include <iostream>

#include "vector2d.cpp"

using namespace std;

struct sphere
{
    vector2d x;
    double r;
    
    // Constructors
    
    sphere()
    {
        this->x = vector2d();
        this->r = 0;
    }
    
    sphere(vector2d x, double r)
    {
        this->x = x;
        this->r = r;
    }
    
    sphere(const sphere & s)
    {
        this->x = s.x;
        this->r = s.r;
    }
    
    // Operators
    
    void operator = (const sphere & s)
    {
        this->x = s.x;
        this->r = s.r;
    }
};

ostream & operator << (ostream & out, sphere s)
{
    out<<"[x = "<<s.x<<", r = "<<s.r<<"]";
    return out;
}

#endif