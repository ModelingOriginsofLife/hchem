#ifndef ENDIANESS_CPP
#define ENDIANESS_CPP

#include "endianess.h"

endianess_converter :: endianess_converter(endianess target_endianess)
{
    union
    {
        uint32_t i;
        char c[4];
    } sysendint = {0x01000000};
    
    if((*sysendint.c))
        this->system_endianess = BIG;
    else
        this->system_endianess = LITTLE;
    
    this->target_endianess = target_endianess;
}
	
template <class T> T endianess_converter :: operator () (T object)
{
    if(system_endianess == target_endianess)
        return object;
    else
    {
        union
        {
            T object;
            char bytes[sizeof(T)];
        } straight, swap;
        
        straight.object = object;
        
        for(unsigned int i = 0; i<sizeof(T); i++)
            swap.bytes[i] = straight.bytes[sizeof(T) - (i + 1)];
        
        return swap.object;
    }
}

endianess_converter net_endian(endianess_converter :: BIG);

#endif