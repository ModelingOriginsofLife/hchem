#ifndef ADDRESS_CPP
#define ADDRESS_CPP

#include "address.h"

#include "data.cpp"

// Constructors

address :: address()
{
    this->sockaddr.sin_port = 0;
    this->sockaddr.sin_addr.s_addr = 0;
}

address :: address(const char * ip, unsigned short port)
{
    this->sockaddr.sin_family = AF_INET;
    this->sockaddr.sin_port = htons(port);
    this->sockaddr.sin_addr.s_addr = inet_addr(ip);
    memset(this->sockaddr.sin_zero, '\0', sizeof(this->sockaddr.sin_zero));
}

address :: address(sockaddr_in s)
{
    this->sockaddr = s;
}

address :: address(const address & a)
{
    this->sockaddr = a.sockaddr;
}

// Getters

unsigned short address :: port()
{
    return ntohs(this->sockaddr.sin_port);
}

const char * address :: ip()
{
    return inet_ntoa(this->sockaddr.sin_addr);
}

// Operators

void address :: operator = (const address & a)
{
    this->sockaddr = a.sockaddr;
}

bool address :: operator == (const address & a)
{
    return this->sockaddr.sin_addr.s_addr == a.sockaddr.sin_addr.s_addr && this->sockaddr.sin_port == a.sockaddr.sin_port;
}

bool address :: operator == (sockaddr_in s)
{
    return this->sockaddr.sin_addr.s_addr == s.sin_addr.s_addr && this->sockaddr.sin_port == s.sin_port;
}

// Casting

address :: operator sockaddr_in()
{
    return this->sockaddr;
}

address :: operator bool()
{
    return (bool) this->sockaddr.sin_port || (bool) this->sockaddr.sin_addr.s_addr;
}

ostream & operator << (ostream & out, address addr)
{
    out<<"{"<<addr.ip()<<", "<<addr.port()<<"}";
    return out;
}
#endif