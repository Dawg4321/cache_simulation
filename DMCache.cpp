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

/* ~~~~~~~~~~~~~~~~~~~~getByte~~~~~~~~~~~~~~~~~~~~

    Simulates a Cache Request in a Direct Mapped CPU Cache.
    NOTE: function attempts to match CPU cache handling as close as possible thus computationally inefficient methods may be used so
    the simulation matches how the hardware operates.  

    ~~ Operation ~~

    Function computes cache requests to determine if the requested address is a hit or a miss.
    In the event of a cache hit, a byte is returned from DMCache object's cache_entries array.
    In the event of a cache miss, four bytes are loaded into corresponding set in cache_entries array from input variable. Requested byte is also returned.
    This will overwrite a previous entry.

    ~~ input variables ~~

    -> int addr = Reqeuested address from the cpu.
    
    -> char input_bytes[4] = Array of bytes stored within specific address.
       As DRAM is not simulated, bytes which are stored in address must be passed into function.
       This is so data can be loaded into the cache in the event of a cache miss. 

    ~~ output variables ~~

    -> char output_to_cpu = variable to store returned byte from cache data request

   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
char DMCache::getByte(int addr, char input_bytes[4]){

    char output_to_cpu; // return variable for byte from cache

    int byte_no = addr&0x3; // gathering byte number (last 2 bits in addr) with "and" operation 
    int set_no = (addr&0x0000FFFC)>>2; // gathering set number (14 bits after byte num in addr) using "and" and shift 2 bits to right (set_no>2)
    int upper_tag_no = (addr&0xFFFF0000)>>16; // gathering tag number (16 bits after set number in addr) using "and" and shift 16 bits to the right (tag>16)
    
    string print_tag; // string to store whether cache request was a hit or a miss for use in result print out
    
    if(cache_entries[set_no].upper_tag == upper_tag_no && !cache_entries[set_no].invalid){
        // cache hit as tag in address matches tag within set number's cache line
        
        print_tag = "hit"; // setting print_tag to hit
                           // this is used when printing getByte request result
        output_to_cpu = cache_entries[set_no].bytes[byte_no]; // set output variable to corresponding byte
                                                              // set_no = which line within way to access
                                                              // byte_no = which byte to access from line
        hit_counter++; // increment hit counter to track this hit
    }
    else{
        // cache miss (matching upper tag was not found within a valid cache entry of corresponding set number)

        print_tag = "miss"; // setting print_tag to miss
                            // this is used when printing getByte request result
        cache_entries[set_no].upper_tag = upper_tag_no; // set cache line to corresponding values
        cache_entries[set_no].invalid = false;          // set_no = which line within way to access

        for(int i = 0; i < 4; i++) // iterate through bytes to load
            cache_entries[set_no].bytes[i] = input_bytes[i];  // copy bytes into address 
                                                              // set_no = which line within way to access

        output_to_cpu = cache_entries[set_no].bytes[byte_no]; // setting return variable to corresponding byte
                                                              // set_no = which line within way to access
                                                              // byte_no = which byte to access from line
        miss_counter++; // increment miss counter to track this miss
    }

    if(hit_counter+miss_counter == 1){ // if this is the first time getByte is called, create table headers 
        string line = "--------------------------------------------------------------"; // line string for printf output to help table format

        printf("****Direct Mapped Cache Request Table****\n"); // print title
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

/* ~~~~~~~~~~~~~~~~~~~~invalidateCache~~~~~~~~~~~~~~~~~~~~

    Invalidates all cache lines in the Direct Mapped CPU Cache.  

    ~~ Operation ~~

    Function resets values on hit and miss counter to 0.
    For loop is used to iterate through each cache line in cache_entries and set the invalid flag to true.
    This means that every entry within the cache will be marked invalid and will not register a hit even if the tag and set numbers match.

   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
void DMCache::invalidateCache(){
    
    hit_counter = 0; // reseting hit counter
    miss_counter = 0; // reseting miss counter

    for(int i = 0; i < sizeof(cache_entries)/sizeof(cache_entries[0]); i++) // iterate through each line within cache
        cache_entries[i].invalid = true; // invalidate cache line
    return;
}

// ~~printSpecs~~
//
// ~~~~~~~~~~~~~~
void DMCache::printSpecs(){
    int set_size = sizeof(cache_entries)/sizeof(cache_entries[0]); // set size is equal to the length of set
    int num_of_sets = 1; // the number of sets is equal to the number of cache ways used
                         // num_of_sets = 1 as it Direct Mapped Cache is equivalent to 1 way set associative cache
    int num_of_cache_lines = set_size*num_of_sets; // the number of cache lines in the cache is equal to the number of sets times the number of ways 
    int cache_size = 4*num_of_cache_lines; // cache size is equal to the number of cache lines times the number of bytes per line (4 bytes per line)

    printf("****Cache Specifications****\n"); // print title 
    printf("Cache Line Size: 4 bytes\n"); // print number of bytes per line
    printf("Total Number of Lines: %d Lines\n", num_of_cache_lines); // print number of lines 
    printf("Number of Ways: %d\n", num_of_sets); // print number of sets
    printf("Number of Sets: %d Lines\n", set_size); // print set size
    printf("Cache Size: %d bytes\n", cache_size); // print cache size
    printf("\n"); // adding extra line for better output spacing 
}