#ifndef VECTOR2D_CPP
#define VECTOR2D_CPP

#include <math.h>
#include <iostream>

using namespace std;

struct vector2d
{
    double x;
    double y;
    
    // Constructors
    
    vector2d()
    {
        this->x = 0;
        this->y = 0;
    }
    
    vector2d(double x, double y)
    {
        this->x = x;
        this->y = y;
    }
    
    vector2d(const vector2d & v)
    {
        this->x = v.x;
        this->y = v.y;
    }
    
    // Operators
    
    vector2d operator + (vector2d b)
    {
        return vector2d(this->x + b.x, this->y + b.y);
    }
    
    void operator += (vector2d b)
    {
        (*this) = (*this) + b;
    }
    
    vector2d operator - (vector2d b)
    {
        return vector2d(this->x - b.x, this->y - b.y);
    }
    
    void operator -= (vector2d b)
    {
        (*this) = (*this) - b;
    }
    
    vector2d operator * (double l)
    {
        return vector2d(this->x * l, this->y * l);
    }
    
    void operator *= (double l)
    {
        (*this) = (*this) * l;
    }
    
    vector2d operator / (double l)
    {
        return vector2d(this->x / l, this->y / l);
    }
    
    void operator /= (double l)
    {
        (*this) = (*this) / l;
    }
    
    double operator * (vector2d b)
    {
        return this->x * b.x + this->y * b.y;
    }
    
    double operator ~ ()
    {
        return (*this) * (*this);
    }
    
    double operator ! ()
    {
        return sqrt(~(*this));
    }
    
    void operator = (const vector2d & v)
    {
        this->x = v.x;
        this->y = v.y;
    }
};

vector2d operator * (double l, vector2d b)
{
    return b * l;
}

ostream & operator << (ostream & out, vector2d v)
{
    out<<"("<<v.x<<", "<<v.y<<")";
    return out;
}

#endif