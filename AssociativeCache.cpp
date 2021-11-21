#include "AssociativeCache.h"

// ~~~Constructor and Destructors~~~

AssociativeCache::AssociativeCache(unsigned int size){ // AssociativeCache constructor
    cache_entries = new AssociativeCacheLine[size]; // allocating AssociativeCacheLine array of user specified size
    set_size = size; // storing length of cache_entries
    
    // initialise all values in cache to invalid
    // this must be done before cache can be used hence it is done upon object creation
    invalidateCache(); 
}

AssociativeCache::~AssociativeCache(){ // AssociativeCache destructor
    delete[] cache_entries; // deallocate cache_entries
}

// ~~~ AssociativeCache Member Functions~~~

/* ~~~~~~~~~~~~~~~~~~~~getByte~~~~~~~~~~~~~~~~~~~~

    Simulates a Cache Request in a Fully Associative CPU Cache.
    NOTE: function attempts to match CPU cache handling as close as possible thus computationally inefficient methods may be used so
    the simulation matches how the hardware operates.  

    ~~ Operation ~~

    Function computes cache requests to determine if the requested address is a hit or a miss.
    In the event of a cache hit, a byte is returned from AssociativeCache object's cache_entries array.
    In the event of a cache miss, four bytes are loaded into cache_entries array from input variable. Requested byte is also returned.
    During each cache miss, all cache entries are pushed up in the cache_entries array as would be done in a hardware implementation 
    of Fully Associative Cache.

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
char AssociativeCache::getByte(int addr, char input_bytes[4]){

    char output_to_cpu; // return variable from function

    int byte_no = addr&0x3; // gathering byte number (last 2 bits in addr) with "and" operation 
    unsigned int tag_no = (addr&0xFFFFFFFC)>>2; // gathering set number (30 bits after byte num in addr) using "and" and shift 2 bits to right (set_no>2)

    string print_tag; // string to store whether cache request was a hit or a miss for use in result print out
    string write_loc; // string to store which cache line is written to for use in result print out

    // iterate through cache slots to check for cache hit
    for(int i = 0; i < set_size; i++){
        if(cache_entries[i].tag == tag_no && !cache_entries[i].invalid){
            // cache hit as tag in address matches tag within cache line from for loop

            print_tag = "hit"; // setting print_tag to hit
            write_loc = "N/A"; // setting write_loc to N/A as cache line is not updated in this instance
                               // these are used when printing getByte request result

            // cache computations 

            output_to_cpu = cache_entries[i].bytes[byte_no]; // set output variable to requested byte
            hit_counter++; // increment hit counter to track this hit
            
            break; // break from loop as hit or miss condition has been met
        }
        else if (i == set_size - 1){
            // cache miss as end of queue reached without finding a matching valid address
            // performance can be improved by declaring cache miss if current address is invalid
            // this is allowed as normal cache operation would ensure that all addresses after first invalid are invalid
            // this performance enhancing feature was not implemented as CPU cache computes the hits and misses in parallel
            // inorder for the for loop to operate similar to a parallel operation, all entries must be checked even if it does not make sense from a performance perspective
            
            // setting strings for print output

            print_tag = "miss"; // setting print_tag to miss
            write_loc = "0 ";   // setting write_loc to 0 as cache line 0 is updated during cache miss
                                // these are used when printing getByte request result

            // cache computations 
            
            // as Associative cache entries act as a FIFO Queue as each cache entry must be pushed up before write
            // this is not an efficient method as each entry must be moved with each miss 
            // this method was used as it is more representative of how hardware cache handles entries on a miss 
            for(int i = set_size - 1; i > 0; i--) // iterate through each cache slot
                cache_entries[i] = cache_entries[i-1]; // move up previous entry to current entry
            
            // append first cache slot with tag 
            cache_entries[0].tag = tag_no;
            // ensure inserted data into cache is valid for reading
            cache_entries[0].invalid = false;
            // copy bytes into address location within cache
            for(int j = 0; j < 4; j++)
                cache_entries[0].bytes[j] = input_bytes[j];
                        
            miss_counter++; // increment miss counter to track this miss 

            break; // break from loop as hit or miss condition has been met
        }
        else{
            // do nothing as current slot in cache is valid but does not have a matching tag
        }
    }

    if(hit_counter+miss_counter == 1){ // if this is the first time getByte is called, create table headers 
        string line = "--------------------------------------------------------------"; // line string for printf output to help table format

        printf("****Associative Cache Request Table****\n"); // print title
        // print line seperator, breaking line into smaller chuncks to match table column width
        printf("+%.3s+%.13s+%.9s+%.11s+%.10s+%.11s+%.4s+%.5s+\n",line.c_str(), line.c_str(), line.c_str(), line.c_str(), line.c_str(), line.c_str(), line.c_str(), line.c_str());
        // print title line of table
        printf("| # |Address (hex)|tag (hex)|byte# (hex)| hit/miss |line loaded|hit#|miss#|\n"); 
        // print line sperator, breaking line into smaller chuncks to match table column width
        printf("+%.3s+%.13s+%.9s+%.11s+%.10s+%.11s+%.4s+%.5s+\n",line.c_str(), line.c_str(), line.c_str(), line.c_str(), line.c_str(), line.c_str(), line.c_str(), line.c_str());
    }
    // print details regarding cache 
    printf("|%3d|   %08x  | %08x|     %1x     |Cache %-4s|   %4s    |%4d|%5d|\n",   hit_counter + miss_counter, // total number of cache queries
                                                                                    addr, // address requested
                                                                                    tag_no, // address tag
                                                                                    byte_no, // byte offset
                                                                                    print_tag.c_str(), // string which prints hit or miss
                                                                                    write_loc.c_str(), // string which prints cache write line
                                                                                    hit_counter, // number of hits
                                                                                    miss_counter // number of misses
                                                                                    );
    return output_to_cpu; // return byte from specified cache location
}

/* ~~~~~~~~~~~~~~~~~~~~invalidateCache~~~~~~~~~~~~~~~~~~~~

    Invalidates all cache lines in the Fully Associative CPU Cache.  

    ~~ Operation ~~

    Function resets values on hit and miss counter to 0.
    For loop is used to iterate through each cache line in cache_entries and set the invalid flag to true.
    This means that every entry within the cache will be marked invalid and will not register a hit even if the tag numbers match.

   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
void AssociativeCache::invalidateCache(){
    hit_counter = 0; // resetting hit counter
    miss_counter = 0; // resetting miss counter

    for(int i = 0; i < set_size; i++) // iterate through each line within cache
        cache_entries[i].invalid = true; // invalidate cache line
    return;
}

/* ~~~~~~~~~~~~~~~~~~~~printSpecs~~~~~~~~~~~~~~~~~~~~

    Simple function which prints out specifications of cache being simulated.

   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
void AssociativeCache::printSpecs(){
    printf("****Cache Specifications****\n"); // print title
    printf("Cache Line Size: 4 bytes\n"); // print number of bytes per cache line
    printf("Total Number of Lines: %d Lines\n", set_size); // print number of lines
    printf("Number of Ways: 1\n"); // print number of sets
    printf("Number of Sets: %d Lines\n", set_size); // print number of sets
    printf("Cache Size: %d bytes\n",4*set_size); // print cache size in bytes
                                                   // multiply by 4 as each cache line has 4 bytes
    printf("\n"); // adding extra line for better output spacing
}