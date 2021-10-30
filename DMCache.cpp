#include "DMCache.h"

// ~~~Constructor and Destructors~~~

DMCache::DMCache(){ // DMCache constructor

    // initialise all values in cache to invalid
    // this must be done before cache can be used hence it is done upon object creation
    invalidateCache();
}

DMCache::~DMCache(){ // DMCache destructor
}

// ~~~DMCache Member Functions~~~

// ~~getByte~~
//
// ~~~~~~~~~~~
char DMCache::getByte(int addr, char input_bytes[4]){

    char output_to_cpu; // return variable for byte from cache

    int byte_no = addr&0x3; // gathering byte number (last 2 bits in addr) with "and" operation 
    int set_no = (addr&0x0000FFFC)>>2; // gathering set number (14 bits after byte num in addr) using "and" and shift 2 bits to right (set_no>2)
    int upper_tag_no = (addr&0xFFFF0000)>>16; // gathering tag number (16 bits after set number in addr) using "and" and shift 16 bits to the right (tag>16)
    
    string print_tag;
    if(cache_entries[set_no].upper_tag == upper_tag_no && !cache_entries[set_no].invalid){
        // cache hit as tag in address matches tag within set number's cache line
        
        print_tag = "hit"; // setting print_tag to hit
                           // this is used when printing getByte request result
        output_to_cpu = cache_entries[set_no].bytes[byte_no]; // set output variable to corresponding byte
                                                              // set_no = which line within block to access
                                                              // byte_no = which byte to access from block
        hit_counter++; // increment hit counter to track this hit
    }
    else{
        // cache miss (matching upper tag was not found within a valid cache entry of corresponding set number)

        print_tag = "miss"; // setting print_tag to miss
                            // this is used when printing getByte request result
        cache_entries[set_no].upper_tag = upper_tag_no; // set cache line to corresponding values
        cache_entries[set_no].invalid = false;          // set_no = which line within block to access

        for(int i = 0; i < 4; i++) // iterate through bytes to load
            cache_entries[set_no].bytes[i] = input_bytes[i];  // copy bytes into address 
                                                              // set_no = which line within block to access

        output_to_cpu = cache_entries[set_no].bytes[byte_no]; // setting return variable to corresponding byte
                                                              // set_no = which line within block to access
                                                              // byte_no = which byte to access from block
        miss_counter++; // increment miss counter to track this miss
    }

    if(hit_counter+miss_counter == 1){ // if this is the first time getByte is called, create table headers 
        string line = "--------------------------------------------------------------"; // line string for printf output to help table format

        printf("Direct Mapped Cache Request Table\n"); // print title
        // print line seperator
        printf("+%.3s+%.13s+%.9s+%.9s+%.11s+%.10s+%.4s+%.5s+\n",line.c_str(), line.c_str(), line.c_str(), line.c_str(), line.c_str(), line.c_str(), line.c_str(), line.c_str());
        // print title line of table
        printf("| # |Address (hex)|tag (hex)|set (hex)|byte# (hex)| hit/miss |hit#|miss#|\n"); 
        // print line sperator
        printf("+%.3s+%.13s+%.9s+%.9s+%.11s+%.10s+%.4s+%.5s+\n",line.c_str(), line.c_str(), line.c_str(), line.c_str(), line.c_str(), line.c_str(), line.c_str(), line.c_str());
    }
    // print details regarding cache 
    printf("|%3d|   %08x  |   %04x  |   %04x  |     %1x     |Cache %-4s|%4d|%5d|\n",hit_counter + miss_counter, // total number of cache queries
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
void DMCache::invalidateCache(){
    
    hit_counter = 0; // reseting hit counter
    miss_counter = 0; // reseting miss counter

    for(int i = 0; i < sizeof(cache_entries)/sizeof(cache_entries[0]); i++) // iterate through each line within cache
        cache_entries[i].invalid = true; // invalidate cache line
    return;
}