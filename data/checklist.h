#ifndef CHECKLIST_H
#define CHECKLIST_H

#include <stdio.h>
#include <string.h>

#define CHECKLIST_CHUNK_SIZE 262144
#define CHECKLIST_CHUNKS 512

using namespace std;

class checklist
{
    struct checklist_chunk
    {
        char * chunk;
        unsigned int count;
        
        checklist_chunk();
        ~checklist_chunk();
        
        void check(unsigned int);
        void decheck(unsigned int);
        
        void clear();
        
        bool operator [] (unsigned int);
    };
    
    checklist_chunk * chunks;
    
    public:
    
    checklist();
    ~checklist();
    
    void check(unsigned int);
    void decheck(unsigned int);
    void clear();
    bool operator [] (unsigned int);
};

#endif