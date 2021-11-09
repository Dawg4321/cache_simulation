#ifndef ASSOCIATIVECACHE_H
#define ASSOCIATIVECACHE_H

#include <stdio.h>
#include <string>

using namespace std;

struct AssociativeCacheLine{
    bool invalid;
    unsigned int tag; // 30 bit tag 
    char bytes[4];
};

class AssociativeCache
{
    public:
        // contructor and destructor
        AssociativeCache(unsigned int size);
        ~AssociativeCache();
        // member functions
        void invalidateCache(); // function to invalidate all entries within cache
        void printSpecs(); // function to print specifications for modelled cache 
        char getByte(int x, char input_bytes[4]); // function to query cache for a specific address value
                                                  // if cache miss occurs, load bytes from input array
    private:
        struct AssociativeCacheLine *cache_entries; // dynamic array to store each cache line
                                                    // Each entry represents a cache line
        int set_size; // size of cache set
        int hit_counter; // counter to track number of cache hits 
        int miss_counter; // counter to track number of cache misses
};

#endif