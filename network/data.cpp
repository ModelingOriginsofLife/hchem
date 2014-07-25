#ifndef DATA_CPP
#define DATA_CPP

#include "data.h"

#include "endianess.cpp"

// Memory

// Constructors
        
data :: memory :: memory(unsigned long nbytes)
{
    if(nbytes)
        this->chunks = nbytes / DATA_CHUNK_LENGTH + (bool) nbytes % DATA_CHUNK_LENGTH;
    else
        this->chunks = 1;
    
    this->bytes = new char[DATA_CHUNK_LENGTH * this->chunks];
    this->references = 0;
}

// Destructors
        
data :: memory :: ~memory()
{
    delete [] this->bytes;
}

    
// Data

// Constructors

data :: data()
{
    this->memory = new struct memory;
    this->memory->references++;
    
    this->beg = (DATA_CHUNK_LENGTH * this->memory->chunks / 2);
    this->len = 0;
}

data :: data(const char * bytes, unsigned long len)
{
    this->memory = new struct memory(len);
    this->memory->references++;
    
    this->beg = (DATA_CHUNK_LENGTH * this->memory->chunks / 2) - (len / 2);
    this->len = len;
    
    memcpy(this->memory->bytes + this->beg, bytes, this->len);
}

data :: data(const char * bytes) : data(bytes, strlen(bytes))
{
}

data :: data(ifstream & f, long int beg = -1, long int end = -1)
{
    f.seekg(0, ios :: end);
    unsigned long file_length = f.tellg();

    if(beg == -1) beg = 0;
    if(end == -1) beg = file_length;
    
    if(beg < 0) beg = 0;
    if(end > file_length) end = file_length;
    
    if(end <= beg)
    {
        this->memory = new struct memory;
        this->memory->references++;
        
        this->beg = (DATA_CHUNK_LENGTH * this->memory->chunks / 2);
        this->len = 0;
    }
    else
    {
        char * bytes = new char[end - beg];
        f.seekg(beg, ios :: beg);
        f.read(bytes, end - beg);
        
        this->memory = new struct memory(end - beg);
        this->memory->references++;
        
        this->beg = (DATA_CHUNK_LENGTH * this->memory->chunks / 2) - ((end - beg) / 2);
        this->len = end - beg;
        
        memcpy(this->memory->bytes + this->beg, bytes, this->len);
        
        delete [] bytes;
    }
}

data :: data(const data & d)
{
    this->memory = d.memory;
    this->beg = d.beg;
    this->len = d.len;
    
    this->memory->references++;
}

// Destructor

data :: ~data()
{
    this->memory->references--;
    if(!(this->memory->references))
        delete this->memory;
}

// Getters

unsigned long data :: length() const
{
    return this->len;
}

template <class T> T data :: read(unsigned int pos) const
{
    if(pos + sizeof(T) <= this->len)
    {
        union
        {
            T object;
            char bytes[sizeof(T)];
        } object;
        
        memcpy(object.bytes, this->memory->bytes + this->beg + pos, sizeof(T));
        
        return net_endian(object.object);
    }
    else
        return T();
}

// Append and prepend

void data :: append(const data & d)
{
    this->memory_setup(0, d.len);
    memcpy(this->memory->bytes + this->beg + this->len, d.memory->bytes + d.beg, d.len);

    this->len += d.len;
}

void data :: append(const char * bytes, unsigned long len)
{
    this->memory_setup(0, len);
    memcpy(this->memory->bytes + this->beg + this->len, bytes, len);
    
    this->len += len;
}

void data :: append(const char * bytes)
{
    this->append(bytes, strlen(bytes));
}

template <class T> void data :: append(T object)
{
    union
    {
        T object;
        char bytes[sizeof(T)];
    } netobject;
    
    netobject.object = net_endian(object);
    this->append(netobject.bytes, sizeof(T));
}

void data :: prepend(const data & d)
{
    this->memory_setup(d.len, 0);
    memcpy(this->memory->bytes + this->beg - d.len, d.memory->bytes + d.beg, d.len);
    
    this->beg -= d.len;
    this->len += d.len;
}

void data :: prepend(const char * bytes, unsigned long len)
{
    this->memory_setup(len, 0);
    memcpy(this->memory->bytes + this->beg - len, bytes, len);
    
    this->beg -= len;
    this->len += len;
}

void data :: prepend(const char * bytes)
{
    this->prepend(bytes, strlen(bytes));
}

template <class T> void data :: prepend(T object)
{
    union
    {
        T object;
        char bytes[sizeof(T)];
    } netobject;
    
    netobject.object = net_endian(object);
    this->prepend(netobject.bytes, sizeof(T));
}

// Operators

void data :: operator = (const data & d)
{
    this->memory->references--;
    if(!(this->memory->references))
        delete this->memory;
    
    this->memory = d.memory;
    this->beg = d.beg;
    this->len = d.len;
    
    this->memory->references++;
}

data data :: operator + (const data & d) const
{
    data r((*this));
    r.append(d);
    return r;
}

void data :: operator += (const data & d)
{
    this->append(d);
}

char & data :: operator [] (unsigned long pos)
{
    this->memory_setup(0, 0);
    return this->memory->bytes[this->beg + pos];
}

data data :: operator () (unsigned long beg, unsigned long end) const
{
    if(end > beg && end <= this->len)
    {
        data r(*this);
        r.beg += beg;
        r.len = end - beg;
        
        return r;
    }
    else
        return data();
}

bool data :: operator == (const data & d) const
{
    if(this->len != d.len)
        return false;
    
    return !((bool) memcmp(this->memory->bytes + this->beg, d.memory->bytes + d.beg, this->len));
}

bool data :: operator != (const data & d) const
{
    return !((*this) == d);
}

// Casting

data :: operator const char * () const
{
    return this->memory->bytes + this->beg;
}

data :: operator char * ()
{
    this->memory_setup(0, 0);
    return this->memory->bytes + this->beg;
}

data :: operator bool ()
{
    return (bool) this->len;
}

// Memory management

void data :: memory_setup(unsigned long head, unsigned long tail)
{
    if(this->memory->references > 1 || this->beg < head || DATA_CHUNK_LENGTH * this->memory->chunks - (this->beg + this->len) < tail)
    {
        struct memory * nmemory = new struct memory(this->len + head + tail);
        
        unsigned long nbeg = (DATA_CHUNK_LENGTH * nmemory->chunks / 2) - ((this->len + head + tail) / 2) + head;
        memcpy(nmemory->bytes + nbeg, this->memory->bytes + this->beg, this->len);
        
        this->beg = nbeg;
        
        this->memory->references--;
        if(!(this->memory->references))
            delete this->memory;
        
        this->memory = nmemory;
        this->memory->references++;
    }
}

ostream & operator << (ostream & out, data d)
{
    out<<"["<<hex;
    for(unsigned int i=0; i<d.length(); i++)
    {
        if(d[i] < 32 || d[i] >126)
            out<<'\\'<<(unsigned int)(unsigned char) d[i]<<" ";
        else
            out<<d[i];
    }
    
    out<<"]"<<dec;
    return out;
}

#endif