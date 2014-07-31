#ifndef COLLISION_HEAP_CPP
#define COLLISION_HEAP_CPP

#include "../dynamics/collision.cpp"

#define HEAP_SIZE 1048576

class collision_heap
{
    collision * collisions;
    unsigned int len;
    
    public:
    
    // Constructors
    
    collision_heap()
    {
        this->collisions = new collision[HEAP_SIZE];
        this->len = 0;
    }
    
    // Destructor
    
    ~collision_heap()
    {
        delete [] this->collisions;
    }
    
    // Getters
    
    unsigned int length()
    {
        return this->len;
    }
    
    // Methods
    
    void push(collision c)
    {
        this->len++;
        this->collisions[this->len] = c;
        
        for(unsigned int i=this->len; i > 1 && this->collisions[i/2].t > this->collisions[i].t; i/=2)
        {
            collision s = this->collisions[i/2];
            this->collisions[i/2] = this->collisions[i];
            this->collisions[i] = s;
        }
    }
    
    collision peek()
    {
        return this->collisions[1];
    }
    
    collision pop()
    {
        collision c = this->collisions[1];
        
        this->collisions[1] = this->collisions[this->len];
        
        this->len--;
        
        unsigned int i = 1;

        while(true)
        {
            unsigned int left = 2 * i;
            unsigned int right = 2 * i + 1;
            unsigned int smallest = i;
            
            if(left <= this->len && this->collisions[smallest].t > this->collisions[left].t)
                smallest = left;
            
            if(right <= this->len && this->collisions[smallest].t > this->collisions[right].t)
                smallest = right;
            
            if(smallest == i)
                break;

            collision s = this->collisions[smallest];
            this->collisions[smallest] = this->collisions[i];
            this->collisions[i] = s;
            
            i = smallest;
        }
        
        return c;
    }
    
    friend int main();
};

#endif