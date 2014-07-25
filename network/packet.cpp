#ifndef PACKET_CPP
#define PACKET_CPP

#include "packet.h"

#include "data.cpp"
#include "address.cpp"

// Constructors

packet :: packet()
{
}

packet :: packet(address source, address destination, data content)
{
    this->source = source;
    this->destination = destination;
    this->content = content;
}

packet :: packet(const packet & p)
{
    this->source = p.source;
    this->destination = p.destination;
    this->content = p.content;
}

// Operators

void packet :: operator = (const packet & p)
{
    this->source = p.source;
    this->destination = p.destination;
    this->content = p.content;
}

bool packet :: operator == (const packet & p)
{
    return this->source == p.source && this->destination == p.destination && this->content == p.content;
}

packet :: operator bool()
{
    return (bool) this->source || (bool) this->destination || (bool) this->content;
}

#endif