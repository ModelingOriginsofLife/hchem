#ifndef EQUATION_CPP
#define EQUATION_CPP

#include <math.h>

#define NEWTON_PRECISION 10

class equation
{
    unsigned int n;
    double z[4];
    
    public:
    
    // Constructors
    
    equation()
    {
        this->n = 0;
    }
    
    equation(double a, double b, double c)
    {
        double delta = pow(b, 2) - 4.0 * a * c;

        if(delta < 0)
            this->n = 0;
        else
        {
            this->n = 2;

            if(b>0)
            {
                this->z[0] = (2.0 * c) / (-b - sqrt(delta));
                this->z[1] = (-b - sqrt(delta)) / (2.0 * a);
            }
            else
            {
                this->z[0] = (-b + sqrt(delta)) / (2.0 * a);
                this->z[1] = (2.0 * c) / (-b + sqrt(delta));
            }
        }
    }
    
    equation(double a, double b, double c, double d)
    {
        double da = 3.0 * a;
        double db = 2.0 * b;
        double dc = c;
        
        equation e(da, db, dc);
        if(e.length())
        {
        }
        else
        {
        }
    }
    
    equation(const equation & e)
    {
        this->n = e.n;
        memcpy(this->z, e.z, sizeof(unsigned int) * 4);
    }
    
    // Getters
    
    unsigned int length()
    {
        return this->n;
    }
    
    // Methods
    
    double newton(double x0, double a, double b, double c, double d)
    {
        double da = 3.0 * a;
        double db = 2.0 * b;
        double dc = c;
        
        double x = x0;
        
        for(unsigned int i=0; i<NEWTON_PRECISION; i++)
        {
            double f = a * pow(x, 3) + b * pow(x, 2) + c * x + d;
            double df = da * pow(x, 2) + db * x + dc;
            
            x -= f / df;
        }
        
        return x;
    }
    
    // Operators
    
    double operator [] (unsigned int i)
    {
        return this->z[i];
    }
    
    void operator = (const equation & e)
    {
        this->n = e.n;
        memcpy(this->z, e.z, sizeof(unsigned int) * 4);
    }
};

#endif