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

void bind_particles(particle & a, particle & b, double l)
{
    bond * n = new bond(a, b, l);
    a.bonds.push_back(n);
    b.bonds.push_back(n);
}

void unbind_particles(bond * n)
{
    for(unsigned int i=0; i<n->a->bonds.size(); i++)
        if(n->a->bonds[i] == n)
        {
            n->a->bonds.erase(n->a->bonds.begin() + i);
            break;
        }
    
    for(unsigned int i=0; i<n->b->bonds.size(); i++)
        if(n->b->bonds[i] == n)
        {
            n->b->bonds.erase(n->b->bonds.begin() + i);
            break;
        }
    
    delete n;
}

#endif