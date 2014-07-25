#ifndef COLLISION_CPP
#define COLLISION_CPP

#include "particle.cpp"
#include "../geometry/segment.cpp"

struct collision
{
    particle * a;
    particle * b;
    segment * s;
    
    double t;
    
    unsigned int version;
    enum {PARTICLE, SEGMENT_A, SEGMENT_B, SEGMENT_MID, CHAIN} type;
    
    // Constructors
    
    collision()
    {
    }
    
    collision(particle & a, particle & b, double tmax, unsigned int version)
    {
        vector2d va = a.p / a.m;
        vector2d vb = b.p / b.m;

        vector2d xa = a.s.x - va * a.t;
        vector2d xb = b.s.x - vb * b.t;
        
        vector2d p = xa - xb;
        vector2d q = va - vb;
        double r = a.s.r + b.s.r;
        
        double A = ~q;
        double B = 2.0 * p * q;
        double C = ~p - pow(r, 2);
        
        double delta = pow(B, 2) - 4.0 * A * C;
        
        if(delta < 0)
        {
            this->t = -1;
            return;
        }
        
        this->t = (-B - sqrt(delta)) / (2.0 * A);

        if(this->t < a.t || this->t < b.t || this->t > tmax)
        {
            this->t = -1;
            return;
        }
        
        this->a = &a;
        this->b = &b;
        
        this->type = PARTICLE;
        this->version = version;
    }
    
    collision(particle & a, segment & s, double tmax, unsigned int version)
    {
        vector2d va = a.p / a.m;
        vector2d xa = a.s.x - va * a.t;
        
        // Check collision with vertex A
        
        vector2d dxA = xa - s.a;

        double AA = ~va;
        double BA = 2.0 * va * dxA;
        double CA = ~dxA - pow(a.s.r, 2);
        
        double deltaA = pow(BA, 2) - 4.0 * AA * CA;
        
        double tA;
        
        if(deltaA < 0)
            tA = -1;
        
        tA = (-BA - sqrt(deltaA)) / (2.0 * AA);
        
        if(tA < a.t || this->t > tmax)
            tA = -1;
        
        // Check collision with vertex B
        
        vector2d dxB = xa - s.b;
        
        double AB = ~va;
        double BB = 2.0 * va * dxB;
        double CB = ~dxB - pow(a.s.r, 2);
        
        double deltaB = pow(BB, 2) - 4.0 * AB * CB;
        
        double tB;
        
        if(deltaB < 0)
            tB = -1;
        
        tB = (-BB - sqrt(deltaB)) / (2.0 * AB);
        
        if(tB < a.t || this->t > tmax)
            tB = -1;
        
        // Check collision mid-segment
        
        vector2d projxa = ((xa - s.a) * s.s) * s.s;
        vector2d projva = (va * s.s) * s.s;
        
        vector2d aMID = xa - s.a - projxa;
        vector2d bMID = va - projva;
        
        double AMID = ~bMID;
        double BMID = 2.0 * aMID * bMID;
        double CMID = ~aMID - pow(a.s.r, 2);
        
        double deltaMID = pow(BMID, 2) - 4.0 * AMID * CMID;
        
        double tMID;
        
        if(deltaMID < 0)
            tMID = -1;
        
        tMID = (-BMID - sqrt(deltaMID)) / (2.0 * AMID);
        
        if(tMID < a.t || this->t > tmax)
            tMID = -1;
        
        
        if(tMID > 0)
        {
            vector2d xaf = xa + va * tMID;
            double imp = (xaf - s.a) * s.s;
            if(imp < 0 || imp > !(s.a - s.b))
                tMID = -1;
        }
        
        
        // Find best solution (if it exists)
        
        this->t = INFINITY;

        if(tA > 0 && tA < this->t)
        {
            this->t = tA;
            this->type = SEGMENT_A;
        }
        
        if(tB > 0 && tB < this->t)
        {
            this->t = tB;
            this->type = SEGMENT_B;
        }
        
        if(tMID > 0 && tMID < this->t)
        {
            this->t = tMID;
            this->type = SEGMENT_MID;
        }
        
        if(this->t == INFINITY)
            this->t = -1;
        
    }
    
    collision(const collision & c)
    {
        this->a = c.a;
        this->b = c.b;
        this->s = c.s;
        
        this->t = c.t;
        this->version = c.version;
        this->type = c.type;
    }
    
    // Operators
    
    void operator = (const collision & c)
    {
        this->a = c.a;
        this->b = c.b;
        this->s = c.s;
        
        this->t = c.t;
        this->version = c.version;
        this->type = c.type;
    }
    
    // Casting
    
    operator bool ()
    {
        return (t >= 0);
    }
};

#endif