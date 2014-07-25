#ifndef DATA_H
#define DATA_H

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <fstream>

#include "endianess.h"

#define DATA_CHUNK_LENGTH 1024

using namespace std;

class data
{
    struct memory
    {
        char * bytes;
        unsigned long chunks;
        unsigned int references;

        memory(unsigned long = 0);
        ~memory();
    };
    
    struct memory * memory;
    unsigned long beg;
    unsigned long len;
    
    public:
    
    data();
    data(const char *, unsigned long);
    data(const char *);
    data(ifstream &, long int, long int);
    data(const data &);
    
    ~data();
  
    unsigned long length() const;
    template <class T> T read(unsigned int) const;
    
    void append(const data &);
    void append(const char *, unsigned long);
    void append(const char *);
    template <class T> void append(T);
    
    void prepend(const data &);
    void prepend(const char *, unsigned long);
    void prepend(const char *);
    template <class T> void prepend(T);
    
    void operator = (const data &);
    data operator + (const data &) const;
    void operator += (const data &);
    char & operator [] (unsigned long);
    data operator () (unsigned long, unsigned long) const;
    bool operator == (const data &) const;
    bool operator != (const data &) const;
    
    operator const char * () const;
    operator char * ();
    operator bool ();
    
    private:
    
    void memory_setup(unsigned long, unsigned long);
};

ostream & operator << (ostream &, data);

#endif