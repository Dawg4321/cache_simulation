#include "TwoWayCache.h"

// ~~~Constructor and Destructors~~~

TwoWayCache::TwoWayCache(){ // TwoWayCache constructor

    // initialise all values in cache to invalid
    // this must be done before cache can be used hence it is done upon object creation
    invalidateCache();
}

TwoWayCache::~TwoWayCache(){ // DM Cache destructor
}

// ~~~TwoWayCache Member Functions~~~

// ~~getByte()~~
// 
//
// ~~~~~~~~~~~~~
char TwoWayCache::getByte(unsigned int addr, char input_bytes[4]){

    char output_to_cpu; // return variable 

    int byte_no = addr&0x3; // gathering byte number (last 2 bits in addr) with "and" operation 
    int set_no = (addr&0x0000FFFC)>>2; // gathering set number (14 bits after byte num in addr) using "and" and shift 2 bits to right (set_no>2)
    int upper_tag_no = (addr&0xFFFF0000)>>16; // and 16 bits

    string print_tag; // string to store whether cache request was a hit or a miss for result printing

    if(cache_entries[0][set_no].upper_tag == upper_tag_no && !cache_entries[0][set_no].invalid){
        // cache hit on block 0
        
        print_tag = "hit B0"; // setting print_tag to hit
                              // this is used when printing getByte request
        output_to_cpu = cache_entries[0][set_no].bytes[byte_no]; // set output variable to corresponding byte from cache

        hit_counter++; // increment hit counter to track this hit
    }
    else if(cache_entries[1][set_no].upper_tag == upper_tag_no && !cache_entries[1][set_no].invalid){
        // cache hit on block 1
        print_tag = "hit B1"; // setting print_tag to hit
                           // this is used when printing getByte request
        output_to_cpu = cache_entries[1][set_no].bytes[byte_no]; // set output variable to corresponding byte from cache

        hit_counter++; // increment hit counter to track this hit
    }
    else{
        // cache miss (matching upper tag was not found within a valid cache entry)
        
        print_tag = "miss"; // setting print_tag to miss
                            // this is used when printing getByte request

        int lru_slot = 0; // variable to help determine which cache slot was the least recently used 
        if(cache_entries[0][set_no].LRU){ // if zeroth cache block was Least Recently Used
            lru_slot = 0; // set lru_slot to 0 so 0th cache slot if modified
            cache_entries[1][set_no].LRU = true; // set cache block 1's corresponding line to Least Recently Used so it will be overwritten in next miss
        }
        else{ // if first cache block was Least Recently Used
            lru_slot = 1; // set lru_slot to 1 so 1st cache slot if modified
            cache_entries[0][set_no].LRU = true;// set cache block 0's corresponding line to Least Recently Used so it will be overwritten in next miss
        }

        cache_entries[lru_slot][set_no].upper_tag = upper_tag_no; // set cache line to corresponding values
        cache_entries[lru_slot][set_no].invalid = false;          // lru_slot = which cache block to access
        cache_entries[lru_slot][set_no].LRU = false;              // set_no = which line within block to access
                                                                  // byte_no = which byte to access from block

        
        for(int i = 0; i < 4; i++) // iterate through bytes to load
            cache_entries[lru_slot][set_no].bytes[i] = input_bytes[i]; // copy bytes into cache

        output_to_cpu =  cache_entries[lru_slot][set_no].bytes[byte_no]; // setting return variable to corresponding byte
                                                                         // lru_slot = which cache block to access
                                                                         // set_no = which line within block to access
                                                                         // byte_no = which byte to access from block
        miss_counter++; // increment miss counter to track this miss
    }
    
    if(hit_counter+miss_counter == 1){ // if this is the first time getByte is called, create table headers 
        string line = "--------------------------------------------------------------"; // line string for printf output to help table format

        printf("2 Way Associative Cache Request Table\n"); // print title
        // print line seperator
        printf("+%.3s+%.13s+%.9s+%.9s+%.11s+%.12s+%.4s+%.5s+\n",line.c_str(), line.c_str(), line.c_str(), line.c_str(), line.c_str(), line.c_str(), line.c_str(), line.c_str());
        // print title line of table
        printf("| # |Address (hex)|tag (hex)|set (hex)|byte# (hex)|  hit/miss  |hit#|miss#|\n"); 
        // print line sperator
        printf("+%.3s+%.13s+%.9s+%.9s+%.11s+%.12s+%.4s+%.5s+\n",line.c_str(), line.c_str(), line.c_str(), line.c_str(), line.c_str(), line.c_str(), line.c_str(), line.c_str());
    }
    // print details regarding cache 
    printf("|%3d|   %08x  |   %04x  |   %04x  |     %1x     |Cache %-6s|%4d|%5d|\n",hit_counter + miss_counter, // total number of cache queries
                                                                                    addr, // address requested
                                                                                    upper_tag_no, // address tag
                                                                                    set_no, // address set number
                                                                                    byte_no, // byte offset
                                                                                    print_tag.c_str(), // string which prints hit or miss
                                                                                    hit_counter, // number of hits
                                                                                    miss_counter // number of misses
                                                                                    );
    return output_to_cpu; // return byte from specified cache location
}

// ~~invalidateCache()~~
// 
// ~~~~~~~~~~~~~~~~~~~~~
void TwoWayCache::invalidateCache(){
    
    hit_counter = 0; // reseting hit counter
    miss_counter = 0; // reseting miss counter

    for(int i = 0; i < 2; i++){ // iterate through outer array (the two cache blocks)
        for(int j = 0; j < sizeof(cache_entries[i])/sizeof(cache_entries[i][0]); j++){ // iterate through each line within cache
            cache_entries[i][j].invalid = true; // invalidate cache line
            if(i == 0) // if the first cache block is being iterated through
                cache_entries[i][j].LRU = true; // ensure 0th cache block is set to least recently used. 
                                                // This is done so that cache block 0 is first used over cache block 1
        }
    }
    return;
}