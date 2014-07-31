#ifndef CHECKLIST_CPP
#define CHECKLIST_CPP

#include "checklist.h"

// Checklist chunk

checklist :: checklist_chunk :: checklist_chunk()
{
    this->chunk = NULL;
    this->count = 0;
}

// Destructors
        
checklist :: checklist_chunk :: ~checklist_chunk()
{
    if(this->count && this->count < CHECKLIST_CHUNK_SIZE * 8)
        delete [] this->chunk;
}

// Setters
        
void checklist :: checklist_chunk :: check(unsigned int i)
{
    if(i > CHECKLIST_CHUNK_SIZE * 8)
        return;
    
    if(this->count == CHECKLIST_CHUNK_SIZE * 8)
        return;
    
    if(!(this->count))
    {
        this->chunk = new char[CHECKLIST_CHUNK_SIZE];
        memset(this->chunk, '\0', CHECKLIST_CHUNK_SIZE);
    }
    
    unsigned int byte_offset = i / 8;
    unsigned int bit_offset = i % 8;
    
    if(!(this->chunk[byte_offset] & (1<<bit_offset)))
    {
        this->chunk[byte_offset] |= (1<<bit_offset);
        this->count ++;
        
        if(this->count == CHECKLIST_CHUNK_SIZE * 8)
            delete [] this->chunk;
    }
}

void checklist :: checklist_chunk :: decheck(unsigned int i)
{
    if(i > CHECKLIST_CHUNK_SIZE * 8)
        return;
    
    if(!(this->count))
        return;
    
    if(this->count == CHECKLIST_CHUNK_SIZE * 8)
    {
        this->chunk = new char[CHECKLIST_CHUNK_SIZE];
        memset(this->chunk, '\1', CHECKLIST_CHUNK_SIZE);
    }
    
    unsigned int byte_offset = i / 8;
    unsigned int bit_offset = i % 8;
    
    if(this->chunk[byte_offset] & (1<<bit_offset))
    {
        this->chunk[byte_offset] &= ~(1<<bit_offset);
        this->count --;
        
        if(!(this->count))
            delete [] this->chunk;
    }
}

// Deleters
        
void checklist :: checklist_chunk :: clear()
{
    if(this->count && this->count < CHECKLIST_CHUNK_SIZE * 8)
        delete [] this->chunk;
    
    this->count = 0;
}

// Operators
        
bool checklist :: checklist_chunk :: operator [] (unsigned int i)
{
    if(i > CHECKLIST_CHUNK_SIZE * 8)
        return false;
    
    if(!(this->count))
        return false;
    
    if(this->count == CHECKLIST_CHUNK_SIZE * 8)
        return true;
    
    unsigned int byte_offset = i / 8;
    unsigned int bit_offset = i % 8;
    
    return this->chunk[byte_offset] & (1<<bit_offset);
}

// Constructors
	
checklist :: checklist()
{
    this->chunks = new checklist_chunk[CHECKLIST_CHUNKS];
}

// Destructors
	
checklist :: ~checklist()
{
    delete [] this->chunks;
}

// Setters
	
void checklist :: check(unsigned int i)
{
    unsigned int chunk_offset = i / (CHECKLIST_CHUNK_SIZE * 8);
    unsigned int bit_offset = i % (CHECKLIST_CHUNK_SIZE * 8);
    
    this->chunks[chunk_offset].check(bit_offset);
}

void checklist :: decheck(unsigned int i)
{
    unsigned int chunk_offset = i / (CHECKLIST_CHUNK_SIZE * 8);
    unsigned int bit_offset = i % (CHECKLIST_CHUNK_SIZE * 8);
    
    this->chunks[chunk_offset].decheck(bit_offset);
}

// Deleters
    
void checklist :: clear()
{
    for(unsigned int i=0; i<CHECKLIST_CHUNKS; i++)
        this->chunks[i].clear();
}

// Operators
	
bool checklist :: operator [] (unsigned int i)
{
    unsigned int chunk_offset = i / (CHECKLIST_CHUNK_SIZE * 8);
    unsigned int bit_offset = i % (CHECKLIST_CHUNK_SIZE * 8);
    
    return this->chunks[chunk_offset][bit_offset];
}

#endif