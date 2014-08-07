#ifndef CHEMISTRY_CPP
#define CHEMISTRY_CPP

struct chemical_type
{
    unsigned int type;
    unsigned int state;
    
    // Constructors
    
    chemical_type()
    {
        this->type = 0;
        this->state = 0;
    }
    
    chemical_type(unsigned int type, unsigned int state)
    {
        this->type = type;
        this->state = state;
    }
    
    chemical_type(const chemical_type & c)
    {
        this->type = c.type;
        this->state = c.state;
    }
    
    // Operators
    
    void operator = (const chemical_type & c)
    {
        this->type = c.type;
        this->state = c.state;
    }
};

class chemical_rule
{
    chemical_type r1;
    bool l1;
    chemical_type r2;
    
    chemical_type p1;
    bool l2;
    chemical_type p2;
};


#endif