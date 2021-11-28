#ifndef DMCACHE_H
#define DMCACHE_H

#include <stdio.h>
#include <string>
#include <sstream>

using namespace std;

/* ~~~ DMCacheLine ~~~
    
    Simple Struct which emulates a direct mapped cache line. 
    This struct is used within an array in the DMCache Class to represent a direct mapped cache.
    As the line is for a 1 way direct mapped cache, no need to include an LRU bit.

    ~~ variables ~~

    -> bool invalid = a true or false value to mark whether the cache line is valid.

    -> char bytes[4] = an array to represent the bytes stored within the cache line.
                       length is 4 meaning 2 bits from the address must be used as an offset.

    -> unsigned int upper_tag = integer which represents the tag correlated with each cache set. 
                                length of tag must be within 16 bits due to 2 bits reserved for byte offset and 14 bytes for set number.
    
   ~~~~~~~~~~~~~~~~~~~*/
struct DMCacheLine{
    bool invalid;
    char bytes[4];
    unsigned short upper_tag; 
    
};

class DMCache
{
    public:
        // contructor and destructor
        DMCache();
        ~DMCache();
        // member functions
        void invalidateCache(); // function to invalidate all entries within cache 
        void printSpecs(); // function to print specifications for modelled cache
        char getByte(int x, char input_bytes[4]); // function to query cache for a specific address value
                                                  // if cache miss occurs, load bytes from input array
    private:
        struct DMCacheLine cache_entries[16384]; // Array representing a single way cache
                                                  // Each entry represents a cache line
        int hit_counter; // counter to track number of cache hits 
        int miss_counter; // counter to track number of cache misses
};

#endif
