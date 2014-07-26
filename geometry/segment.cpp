#ifndef SEGMENT_CPP
#define SEGMENT_CPP

#include <iostream>

#include "vector2d.cpp"

using namespace std;

struct segment
{
    vector2d a;
    vector2d b;
    
    vector2d s;
    
    // Constructors
    
    segment()
    {
        this->a = vector2d();
        this->b = vector2d();
        
        this->s = (this->b - this->a) / !(this->b - this->a);
    }
    
    segment(vector2d a, vector2d b)
    {
        this->a = a;
        this->b = b;
        
        this->s = (this->b - this->a) / !(this->b - this->a);
    }
    
    segment(const segment & s)
    {
        this->a = s.a;
        this->b = s.b;
        
        this->s = (this->b - this->a) / !(this->b - this->a);
    }
    
    // Operators
    
    void operator = (const segment & s)
    {
        this->a = s.a;
        this->b = s.b;
        
        this->s = (this->b - this->a) / !(this->b - this->a);
    }
};

ostream & operator << (ostream & out, segment s)
{
    out<<"[a = "<<s.a<<", b = "<<s.b<<"]";
    return out;
}

#endif