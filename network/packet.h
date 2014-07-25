#ifndef PACKET_H
#define PACKET_H

#include "data.h"
#include "address.h"

enum packet_type {HP, LP, ACK};

struct packet
{
    address source;
    address destination;
    data content;
    
    packet();
    packet(address, address, data);
    packet(const packet &);
    
    void operator = (const packet &);
    bool operator == (const packet &);
    
    operator bool();
};
#endif