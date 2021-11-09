#ifndef TWOWAYCACHE_H
#define TWOWAYCACHE_H

#include <stdio.h>
#include <string>

using namespace std;

struct TwoWayCacheLine{
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
        void printSpecs(); // function to print specifications for modelled cache 
        char getByte(unsigned int address, char input_bytes[4]); // function to query cache for a specific address value
                                                                 // if cache miss occurs, load bytes from input array
    private:
        struct TwoWayCacheLine cache_entries[2][16384]; // Multi-dimensional array holding cache entries
                                                         // each slot of the outer dimension represents each way
                                                         // the inner array represents the cache lines in each way
        int hit_counter; // counter to track number of cache hits 
        int miss_counter; // counter to track number of cache misses
};

#endif
 