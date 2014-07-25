#ifndef ADDRESS_H
#define ADDRESS_H

#include <arpa/inet.h>

#include "data.h"

struct address
{
    sockaddr_in sockaddr;
    address();
    address(const char *, unsigned short);
    address(sockaddr_in);
    address(const address &);
    
    unsigned short port();
    const char * ip();
    
    void operator = (const address &);
    bool operator == (const address &);
    bool operator == (sockaddr_in);
    
    operator sockaddr_in();
    operator bool();
};

ostream & operator << (ostream &, address);

#endif