#ifndef DMCACHE_H
#define DMCACHE_H

#include <stdio.h>
#include <stdbool.h>

using namespace std;

struct DMCacheLine{
    int LRU; 
    bool invalid;
    unsigned short upper_tag; 
    char bytes[4];
};

class DMCache
{
    public:
        DMCache();
        ~DMCache();
        char getByte(int x, bool *cache_hit, char input_bytes[4]);
    private:
        struct DMCacheLine cache_entries[16384];
};

#endif
