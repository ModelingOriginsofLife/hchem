#ifndef EVENT_HEAP_CPP
#define EVENT_HEAP_CPP

#include "../dynamics/event.cpp"

#define HEAP_SIZE 1048576

class event_heap
{
    event * events;
    unsigned int len;
    
    public:
    
    // Constructors
    
    event_heap()
    {
        this->events = new event[HEAP_SIZE];
        this->len = 0;
    }
    
    // Destructor
    
    ~event_heap()
    {
        delete [] this->events;
    }
    
    // Getters
    
    unsigned int length()
    {
        return this->len;
    }
    
    // Methods
    
    void push(event c)
    {
        this->len++;
        this->events[this->len] = c;
        
        for(unsigned int i=this->len; i > 1 && this->events[i/2].t > this->events[i].t; i/=2)
        {
            event s = this->events[i/2];
            this->events[i/2] = this->events[i];
            this->events[i] = s;
        }
    }
    
    event peek()
    {
        return this->events[1];
    }
    
    event pop()
    {
        event c = this->events[1];
        
        this->events[1] = this->events[this->len];
        
        this->len--;
        
        unsigned int i = 1;

        while(true)
        {
            unsigned int left = 2 * i;
            unsigned int right = 2 * i + 1;
            unsigned int smallest = i;
            
            if(left <= this->len && this->events[smallest].t > this->events[left].t)
                smallest = left;
            
            if(right <= this->len && this->events[smallest].t > this->events[right].t)
                smallest = right;
            
            if(smallest == i)
                break;

            event s = this->events[smallest];
            this->events[smallest] = this->events[i];
            this->events[i] = s;
            
            i = smallest;
        }
        
        return c;
    }
    
    friend int main();
};

#endif