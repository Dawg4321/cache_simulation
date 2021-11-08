#include "TwoWayCache.h"

// ~~~Constructor and Destructors~~~

TwoWayCache::TwoWayCache(){ // TwoWayCache constructor

    // initialise all values in cache to invalid
    // this must be done before cache can be used hence it is done upon object creation
    invalidateCache();
}

TwoWayCache::~TwoWayCache(){ // TwoWayCache destructor
}

// ~~~TwoWayCache Member Functions~~~

// ~~getByte~~
// 
//
// ~~~~~~~~~~~
char TwoWayCache::getByte(unsigned int addr, char input_bytes[4]){

    char output_to_cpu; // return variable for byte from cache

    int byte_no = addr&0x3; // gathering byte number (last 2 bits in addr) with "and" operation 
    int set_no = (addr&0xFFFC)>>2; // gathering set number (14 bits after byte num in addr) using "and" and shift 2 bits to right (set_no>2)
    int upper_tag_no = (addr&0xFFFF0000)>>16; // and 16 bits

    string print_tag; // string to store whether cache request was a hit or a miss for result printing
    string write_loc; // string to store which cache line is written to for use in result print out

    if(cache_entries[0][set_no].upper_tag == upper_tag_no && !cache_entries[0][set_no].invalid){
        // cache hit on block 0 as tag in address matches tag within cache line from set number
        
        print_tag = "hit B0"; // setting print_tag to hit
        write_loc = "hit B0"; // setting print_tag to hit
                              // this is used when printing getByte request result
        
        output_to_cpu = cache_entries[0][set_no].bytes[byte_no]; // set output variable to corresponding byte from cache

        hit_counter++; // increment hit counter to track this hit
    }
    else if(cache_entries[1][set_no].upper_tag == upper_tag_no && !cache_entries[1][set_no].invalid){
        // cache hit on block 1 as tag in address matches tag within set number's cache line

        print_tag = "hit B1"; // setting print_tag to hit
                              // this is used when printing getByte request result
        output_to_cpu = cache_entries[1][set_no].bytes[byte_no]; // set output variable to corresponding byte from cache

        hit_counter++; // increment hit counter to track this hit
    }
    else{
        // cache miss (matching upper tag was not found within a valid cache entry of corresponding set number)
        
        print_tag = "miss"; // setting print_tag to miss
                            // this is used when printing getByte request result

        int lru_slot = 0; // variable to help determine which cache slot was the least recently used 
        if(cache_entries[0][set_no].LRU){ // if zeroth cache block was Least Recently Used
            lru_slot = 0; // set lru_slot to 0 so 0th cache slot if modified
            cache_entries[1][set_no].LRU = true; // set cache block 1's corresponding line to Least Recently Used 
                                                 // so it will be overwritten in next miss
        }
        else{ // if first cache block was Least Recently Used
            lru_slot = 1; // set lru_slot to 1 so 1st cache slot if modified
            cache_entries[0][set_no].LRU = true; // set cache block 0's corresponding line to Least Recently Used
                                                 // so it will be overwritten in next miss
        }

        cache_entries[lru_slot][set_no].upper_tag = upper_tag_no; // setting cache line to corresponding values
        cache_entries[lru_slot][set_no].invalid = false;          // lru_slot = which cache block to access
        cache_entries[lru_slot][set_no].LRU = false;              // set_no = which line within block to access

        
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

        printf("****2 Way Set Associative Cache Request Table*****\n"); // print title
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

// ~~invalidateCache~~
// 
// ~~~~~~~~~~~~~~~~~~~
void TwoWayCache::invalidateCache(){
    
    hit_counter = 0; // resetting hit counter
    miss_counter = 0; // resetting miss counter

    for(int i = 0; i < 2; i++){ // iterate through outer array (iterate through the two cache blocks)
        for(int j = 0; j < sizeof(cache_entries[i])/sizeof(cache_entries[i][0]); j++){ // iterate through each line within cache
            cache_entries[i][j].invalid = true; // invalidate cache line
            if(i == 0) // if the first cache block is being iterated through
                cache_entries[i][j].LRU = true; // ensure 0th cache block is set to least recently used. 
                                                // This is done to ensure that cache block 0 is the first block to be overwritten
        }
    }
    return;
}

// ~~printSpecs~~
//
// ~~~~~~~~~~~~~~
void TwoWayCache::printSpecs(){
    int set_size = sizeof(cache_entries[0])/sizeof(cache_entries[0][0]); // set size is equal to the length of each way.
                                                                         // in terms of the multidimensional array, this is the length of the inner array
    int num_of_sets = sizeof(cache_entries)/sizeof(cache_entries[0]); // the number of sets is equal to the number of cache ways used
                                                                      // in terms of the multidimensional array, this is the length of the outer array
    int num_of_cache_lines = set_size*num_of_sets; // the number of cache lines in the cache is equal to the number of sets times the number of ways 
    int cache_size = 4*num_of_cache_lines; // cache size is equal to the number of cache lines times the number of bytes per line (4 bytes per line)

    printf("****Cache Specifications****\n"); // print title 
    printf("Cache Line Size: 4 bytes\n"); // print number of bytes per line
    printf("Total Number of Lines: %d Lines\n", num_of_cache_lines); // print number of lines 
    printf("Number of Ways: %d\n", num_of_sets); // print number of sets
    printf("Number of Sets: %d Lines\n", set_size); // print set size
    printf("Cache Size: %d bytes\n", cache_size); // print cache size
    printf("****\n");
}