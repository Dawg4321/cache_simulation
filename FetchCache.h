struct FetchDMCache{
    bool LRU;
    bool invalid;
    unsigned short upper_tag; 
    char bytes[4];
};
