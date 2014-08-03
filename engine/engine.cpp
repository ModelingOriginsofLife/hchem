#ifndef ENGINE_CPP
#define ENGINE_CPP

#include "zone.cpp"
#include "../data/event_heap.cpp"
#include "../data/checklist.cpp"

class engine
{
    vector <particle> particles;
    vector <bond> bonds;
    vector <segment> segments;
    
    unsigned int size;
    zone ** zones;

    struct
    {
        vector <unsigned int> vec;
        checklist check;
    } active_zones;
    
    unsigned int event_id;
    event_heap events;
    
    // Constructors
    
    engine(unsigned int size)
    {
        this->size = size;

        this->zones = new zone * [this->size];
        for(unsigned int i=0; i<this->size; i++)
            this->zones[i] = new zone[this->size];   
    }
};

#endif