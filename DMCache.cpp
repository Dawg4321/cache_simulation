#include "DMCache.h"

DMCache::DMCache(){ // DMCache constructor

    // initialise all values in cache to invalid
    // this must be done before cache can be used hence it is done upon object creation
    for(int i =0; i < sizeof(cache_entries)/sizeof(cache_entries[0]); i++)
        cache_entries[i].invalid = true;
}

DMCache::~DMCache(){ // DM Cache destructor
}

// ~~~ DMCache::DirectMap ~~~
//
//
// ~~~~~~~~~~~~~~~~~~~~~~~~~~
char DMCache::getByte(int addr, bool *cache_hit, char input_bytes[4]){

    char output_to_cpu; // return variable 

    int byte_no = addr&0x3; // gathering byte number (last 2 bits in addr) with "and" operation 
    int set_no = (addr&0x0000FFFC)>>2; // gathering set number (14 bits after byte num in addr) using "and" and shift 2 bits to right (set_no>2)
    int upper_tag_no = (addr&0xFFFF0000)>>16; // and 16 bits

    if(cache_entries[set_no].upper_tag == upper_tag_no && !cache_entries[set_no].invalid){
        // cache hit
        output_to_cpu = cache_entries[set_no].bytes[byte_no]; // set output variable to corresponding byte
        printf("cache hit\n");

        // returning cache hit to main
        *cache_hit = true;
    }
    else{
        // cache miss

        // set address to corresponding values
        cache_entries[set_no].upper_tag = upper_tag_no;
        cache_entries[set_no].invalid = false;

        // copy bytes into address
        for(int i = 0; i < 4; i++)
            cache_entries[set_no].bytes[i] = input_bytes[i];   
        printf("cache miss\n");
        
        // returning cache miss to main
        *cache_hit = false;
        // set output variable to 0 as cac
        output_to_cpu =  0;
    }
    return output_to_cpu;
}