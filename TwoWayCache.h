#ifndef TWOWAYCACHE_H
#define TWOWAYCACHE_H

#include <stdio.h>
#include <stdbool.h>

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
        TwoWayCache();
        ~TwoWayCache();
        void invalidateCache();
        char getByte(int x, bool *cache_hit, char input_bytes[4]);
    private:
        struct TwoWayCacheLine cache_entries[2][16384];
};

#endif
 