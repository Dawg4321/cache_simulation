#ifndef ASSOCIATIVECACHE_H
#define ASSOCIATIVECACHE_H

#include <stdio.h>
#include <string>
#include "Cache.h"

using namespace std;

/* ~~~ AssociativeCacheLine ~~~
    
    Simple Struct which emulates an associative cache line. 
    This struct is used within an array in the AssociativeCache Class to represent a fully associative cache.
    As the line is for a fully associative cache, no need to include a set number or LRU bit.

    ~~ variables ~~

    -> bool invalid = a true or false value to mark whether the cache line is valid.

    -> char bytes[4] = an array to represent the bytes stored within the cache line.
                       length is 4 meaning 2 bits from the address must be used as an offset.

    -> unsigned int tag = integer which represents the tag correlated with each cache line. 
                          length of values must be a maximum of 30 bits due to 2 bits reserved for byte offset.
    
   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
struct AssociativeCacheLine{
    bool invalid; // cache valid bit
    unsigned int tag; // 30 bit tag due to 2 bits used for offset
    char bytes[4]; // bytes stroed in cache line
};

class AssociativeCache: public Cache
{
    public:
        // contructor and destructor
        AssociativeCache(unsigned int size);
        ~AssociativeCache();
        // overwritten functions
        void invalidateCache(); // function to invalidate all entries within cache
        void printSpecs(); // function to print specifications for modelled cache 
        char getByte(int addr, char input_bytes[4]); // function to query cache for a specific address value
                                                  // if cache miss occurs, load bytes from input array
    private:
        struct AssociativeCacheLine *cache_entries; // dynamic array to store each cache line
                                                    // Each entry represents a cache line
        int num_line; // size of cache lines
};

#endif