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
        // contructor and destructor
        TwoWayCache();
        ~TwoWayCache();
        // member functions
        void invalidateCache(); // function to invalidate all entries within cache
        char getByte(unsigned int address, char input_bytes[4]); // function to query cache for a specific address value
                                                                 // if cache miss occurs, load bytes from input array
    private:
        struct TwoWayCacheEntry cache_entries[2][16384]; // Multi-dimensional array holding cache entries
                                                         // each slot of the outer dimension represents each cache block
                                                         // the inner array represents each cache line in a block
        int hit_counter; // counter to track number of cache hits 
        int miss_counter; // counter to track number of cache misses
};

#endif
 