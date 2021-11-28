#ifndef CACHE_H
#define CACHE_H

using namespace std;

class Cache
{
    public:
        // contructor and destructor
        Cache(){}
        virtual ~Cache(){}
        // member functions
        virtual void invalidateCache(){} // function to invalidate all entries within cache
        virtual void printSpecs(){} // function to print specifications for modelled cache 
        virtual char getByte(int addr, char input_bytes[4]){return 0;} // function to query cache for a specific address value
                                                                       // if cache miss occurs, load bytes from input array
    protected:

        int hit_counter; // counter to track number of cache hits 
        int miss_counter; // counter to track number of cache misses
};

#endif