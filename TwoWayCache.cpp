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

/* ~~~~~~~~~~~~~~~~~~~~getByte~~~~~~~~~~~~~~~~~~~~

    Simulates a Cache Request in a Two Way Set Associative CPU Cache.
    NOTE: function attempts to match CPU cache handling as close as possible thus computationally inefficient methods may be used so
    the simulation matches how the hardware operates.  

    ~~ Operation ~~

    Function computes cache requests to determine if the requested address is a hit or a miss.
    In the event of a cache hit, a byte is returned from TwoWayCache object's cache_entries array.
    In the event of a cache miss, four bytes are loaded into corresponding set in cache_entries array from input variable. Requested byte is also returned.
    If the cache misses and both sets for the requested set number are in use, the cache will overwrite on a Least Recently Used basis.
    For example, if the zeroth way part of the set was the least recently used cache. It will be overwrited next on a cache miss 
    with the corresponding set number 

    During each request, the details of the request are outputed in a table format using printf. 
    If the number of requests is 0, the titles for the table will be printed. 

    ~~ input variables ~~

    -> int addr = Reqeuested address from the cpu.
    
    -> char input_bytes[4] = Array of bytes stored within specific address.
       As DRAM is not simulated, bytes which are stored in address must be passed into function.
       This is so data can be loaded into the cache in the event of a cache miss. 

    ~~ output variables ~~

    -> char output_to_cpu = variable to store returned byte from cache data request

   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
char TwoWayCache::getByte(unsigned int addr, char input_bytes[4]){

    char output_to_cpu; // return variable for byte from cache

    int byte_no = addr&0x3; // gathering byte number (last 2 bits in addr) with "and" operation 
    int set_no = (addr&0xFFFC)>>2; // gathering set number (14 bits after byte num in addr) using "and" and shift 2 bits to right (set_no>2)
    int upper_tag_no = (addr&0xFFFF0000)>>16; // and 16 bits

    stringstream ss; // stringstream object used for converting hex int to hex string

    string print_tag; // string to store whether cache request was a hit or a miss for result printing
    string write_loc; // string to store which cache line is written to for use in result print out

    if(cache_entries[0][set_no].upper_tag == upper_tag_no && !cache_entries[0][set_no].invalid){
        // cache hit on way 0 as tag in address matches tag within cache line from set number
        
        // setting strings for print output

        print_tag = "hit W0"; // setting print_tag to hit
        write_loc = "N/A  "; // setting write_loc to N/A as cache line is not updated in this instance due to miss
                           // these are used when printing getByte request result

        // cache computations 

        output_to_cpu = cache_entries[0][set_no].bytes[byte_no]; // set output variable to corresponding byte from cache

        hit_counter++; // increment hit counter to track this hit
    }
    else if(cache_entries[1][set_no].upper_tag == upper_tag_no && !cache_entries[1][set_no].invalid){
        // cache hit on way 1 as tag in address matches tag within set number's cache line

        // setting strings for print output

        print_tag = "hit W1"; // setting print_tag to hit
        write_loc = "N/A  "; // setting write_loc to N/A as cache line is not updated in this instance due to miss
                           // these are used when printing getByte request result

        // cache computations 

        output_to_cpu = cache_entries[1][set_no].bytes[byte_no]; // set output variable to corresponding byte from cache

        hit_counter++; // increment hit counter to track this hit
    }
    else{
        // cache miss (matching upper tag was not found within a valid cache entry of corresponding set number)
        
        // setting strings for print output

        print_tag = "miss"; // setting print_tag to miss
        ss << hex << set_no; // converting hex value of set number into string with stringstream
        write_loc = ss.str();   // setting write_loc to set number from stringstream as this cache line is updated during cache miss
                                // these are used when printing getByte request result

        // cache computations 

        int lru_way = 0; // variable to help determine which cache way was the least recently used

        if(cache_entries[0][set_no].LRU){ // if set on zeroth cache way was Least Recently Used
            lru_way = 0; // set lru_way to 0 so 0th cache way is modified
            cache_entries[1][set_no].LRU = true; // set cache ways 1's corresponding line to Least Recently Used 
                                                 // so it will be overwritten in next miss
        }
        else{ // if set on first cache way was Least Recently Used
            lru_way = 1; // set lru_way to 1 so 1st cache way is modified
            cache_entries[0][set_no].LRU = true; // set cache way 0 's corresponding line to Least Recently Used
                                                 // so it will be overwritten in next miss
        }

        write_loc = write_loc + " W" + to_string(lru_way); // concatinating write_loc with way number

        cache_entries[lru_way][set_no].upper_tag = upper_tag_no; // setting cache line to corresponding values
        cache_entries[lru_way][set_no].invalid = false;          // lru_way = which cache way to access
        cache_entries[lru_way][set_no].LRU = false;              // set_no = which line within way to access

        
        for(int i = 0; i < 4; i++) // iterate through bytes to load
            cache_entries[lru_way][set_no].bytes[i] = input_bytes[i]; // copy bytes into cache

        output_to_cpu =  cache_entries[lru_way][set_no].bytes[byte_no]; // setting return variable to corresponding byte
                                                                         // lru_way = which cache way to access
                                                                         // set_no = which line within way to access
                                                                         // byte_no = which byte to access from line
        miss_counter++; // increment miss counter to track this miss
    }
    
    if(hit_counter+miss_counter == 1){ // if this is the first time getByte is called, create table headers 
        string line = "--------------------------------------------------------------"; // line string for printf output to help table format

        printf("****2 Way Set Associative Cache Request Table*****\n"); // print title
        // print line seperator, breaking line into smaller chuncks to match table column width
        printf("+%.3s+%.13s+%.9s+%.9s+%.11s+%.12s+%.11s+%.4s+%.5s+\n",line.c_str(), line.c_str(), line.c_str(), line.c_str(), line.c_str(), line.c_str(), line.c_str(), line.c_str(), line.c_str());
        // print title line of table
        printf("| # |Address (hex)|tag (hex)|set (hex)|byte# (hex)|  hit/miss  |line loaded|hit#|miss#|\n"); 
        // print line sperator, breaking line into smaller chuncks to match table column width
        printf("+%.3s+%.13s+%.9s+%.9s+%.11s+%.12s+%.11s+%.4s+%.5s+\n",line.c_str(), line.c_str(), line.c_str(), line.c_str(), line.c_str(), line.c_str(), line.c_str(), line.c_str(), line.c_str());
    }
    // print details regarding cache 

    printf("|%3d|   %08x  |   %04x  |   %04x  |     %1x     |Cache %-6s|  %7s  |%4d|%5d|\n",hit_counter + miss_counter, // total number of cache queries
                                                                                            addr, // address requested
                                                                                            upper_tag_no, // address tag
                                                                                            set_no, // address set number
                                                                                            byte_no, // byte offset
                                                                                            print_tag.c_str(), // string which prints hit or miss
                                                                                            write_loc.c_str(), // string which prints cache write line
                                                                                            hit_counter, // number of hits
                                                                                            miss_counter // number of misses
                                                                                            );
    return output_to_cpu; // return byte from specified cache location
}

/* ~~~~~~~~~~~~~~~~~~~~invalidateCache~~~~~~~~~~~~~~~~~~~~

    Invalidates all cache lines in the Two Way Set Associative CPU Cache.  

    ~~ Operation ~~

    Function resets values on hit and miss counter to 0.
    For loops is used to iterates through each cache way and cache line in cache_entries inorder to set the invalid flag to true.
    This means that every entry within the cache will be marked invalid and will not register a hit even if the tag and set numbers match.

   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
void TwoWayCache::invalidateCache(){
    
    hit_counter = 0; // resetting hit counter
    miss_counter = 0; // resetting miss counter

    for(int i = 0; i < 2; i++){ // iterate through outer array (iterate through the two cache ways)
        for(int j = 0; j < sizeof(cache_entries[i])/sizeof(cache_entries[i][0]); j++){ // iterate through each line within cache
            cache_entries[i][j].invalid = true; // invalidate cache line
            if(i == 0) // if the first cache way is being iterated through
                cache_entries[i][j].LRU = true; // ensure 0th cache way is set to least recently used. 
                                                // This is done to ensure that cache way 0 is the first way to be overwritten with LRU
        }
    }
    return;
}

/* ~~~~~~~~~~~~~~~~~~~~printSpecs~~~~~~~~~~~~~~~~~~~~

    Simple function which prints out specifications of cache being simulated.
    
    ~~ Operation ~~

    Simple Cache calculations are computed to determine specifics details.
    Specifications are then outputed using printf.

   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
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
    printf("Number of Sets: %d\n", set_size); // print set size
    printf("Cache Size: %d bytes\n", cache_size); // print cache size
    printf("\n"); // adding extra line for better output spacing
}