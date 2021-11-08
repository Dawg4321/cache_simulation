struct CacheFetching{
    bool LRU;
    bool invalid;
    unsigned short tag; 
    char bytes[4];
};
struct CacheFetching[64384];
{
    /* data */
};

class cache
{
    public:
        cache(/* args */);
        ~cache();
        
    private:

};
// direct mapped: when you send address into cache the cache addresses are directly mapped