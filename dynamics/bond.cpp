#ifndef BOND_CPP
#define BOND_CPP

#include "particle.cpp"

struct bond
{
    particle * a;
    particle * b;
    double l;
    
    // Constructors
    
    bond()
    {
    }
    
    bond(particle & a, particle & b, double l)
    {
        this->a = &a;
        this->b = &b;
        this->l = l;
    }
    
    bond(const bond & b)
    {
        this->a = b.a;
        this->b = b.b;
        this->l = b.l;
    }
    
    // Operators
    
    void operator = (const bond & b)
    {
        this->a = b.a;
        this->b = b.b;
        this->l = b.l;
    }
};

#endif