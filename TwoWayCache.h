#ifndef TWOWAYCACHE_H
#define TWOWAYCACHE_H

#include <stdio.h>
#include <string>
#include <sstream>

using namespace std;

/* ~~~ TwoWayCacheLine ~~~
    
    Simple Struct which emulates a Two Way Set Associative cache line. 
    This struct is used within an array in the TwoWayCache Class to represent a two way set associative cache.

    ~~ variables ~~

    -> bool invalid = a true or false value to mark whether the cache line is valid.

    -> char bytes[4] = an array to represent the bytes stored within the cache line.
                       length is 4 meaning 2 bits from the address must be used as an offset.

    -> unsigned int upper_tag = integer which represents the tag correlated with each cache set. 
                                length of tag must be within 16 bits due to 2 bits reserved for byte offset and 14 bytes for set number.
    
   ~~~~~~~~~~~~~~~~~~~*/

struct TwoWayCacheLine{
    bool LRU; 
    bool invalid;
    char bytes[4];
    unsigned short upper_tag; 
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
 