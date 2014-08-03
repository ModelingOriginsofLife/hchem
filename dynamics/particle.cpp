#ifndef PARTICLE_CPP
#define PARTICLE_CPP

#include <iostream>
#include <vector>

#include "../geometry/vector2d.cpp"
#include "../geometry/sphere.cpp"

using namespace std;

struct particle
{
    sphere s;
    double m;
    vector2d p;
    double t;
    
    // Constructors
    
    particle()
    {
        this->s = sphere();
        this->m = 0;
        this->p = vector2d();
        this->t = 0;
    }
    
    particle(sphere s, double m, vector2d p)
    {
        this->s = s;
        this->m = m;
        this->p = p;
        this->t = 0;
    }
    
    particle(const particle & p)
    {
        this->s = p.s;
        this->m = p.m;
        this->p = p.p;
        this->t = p.t;
    }
    
    // Operators
    
    void operator = (const particle & p)
    {
        this->s = p.s;
        this->m = p.m;
        this->p = p.p;
        this->t = p.t;
    }
};

ostream & operator << (ostream & out, particle p)
{
    out<<"{ s = "<<p.s<<", m = "<<p.m<<", p = "<<p.p<<", t = "<<p.t<<"}";
    return out;
}


#endif