#ifndef TWOWAYCACHE_H
#define TWOWAYCACHE_H

#include <stdio.h>
#include <string>

using namespace std;

struct TwoWayCacheEntry{
    int LRU; 
    bool invalid;
    unsigned short upper_tag; 
    char bytes[4];
};

class TwoWayCache
{
    public:
        TwoWayCache();
        ~TwoWayCache();
        void invalidateCache();
        char getByte(unsigned int x, char input_bytes[4]);
    private:
        struct TwoWayCacheEntry cache_entries[2][16384];

        int hit_counter; // counter to track number of cache hits 
        int miss_counter; // counter to track number of cache misses
};

#endif
 