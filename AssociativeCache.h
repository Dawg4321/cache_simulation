#ifndef ASSOCIATIVECACHE_H
#define ASSOCIATIVECACHE_H

#include <stdio.h>

using namespace std;

struct AssociativeCacheLine{
    bool invalid;
    int tag; // 30 bit tag 
    char bytes[4];
};

class AssociativeCache
{
    public:
        AssociativeCache(int size);
        ~AssociativeCache();
        
        char getByte(int x, char input_bytes[4]);
        void invalidateCache();
    private:
        struct AssociativeCacheLine *cache_entries; // dynamic array to store each cache line

        int block_size; // size of cache block

        int hit_counter; // counter to track number of cache hits 
        int miss_counter; // counter to track number of cache misses
};

#endif