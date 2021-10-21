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
        
        char getByte(int x, bool *cache_hit, char input_bytes[4]);
        void loadCacheSlot(int cache_slot, int tag, char bytes[4]);
    private:
        struct AssociativeCacheLine *cache_block1; // dynamic array to store each cache line
        int block_size; // size of cache block

        int fifo_tracker; // simple counter to track which line has been in the cache for the longest time 
        
        int hit_counter; // counter to track number of cache hits 
        int miss_counter; // counter to track number of cache misses
};

#endif