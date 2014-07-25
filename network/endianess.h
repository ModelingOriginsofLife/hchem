#ifndef ENDIANESS_H
#define ENDIANESS_H

#include <stdint.h>

struct endianess_converter
{
    enum endianess {BIG, LITTLE};
	
	endianess system_endianess;
	endianess target_endianess;
    
    endianess_converter(endianess);
    
    template <class T> T operator () (T);

};

#endif