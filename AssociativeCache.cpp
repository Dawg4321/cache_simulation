#include "AssociativeCache.h"

// ~~~Constructor and Destructors~~~

AssociativeCache::AssociativeCache(int size){
    cache_entries = new AssociativeCacheLine[size];
    block_size = size;
    invalidateCache();
}

AssociativeCache::~AssociativeCache(){
    delete[] cache_entries;
}

// ~~~TwoWayCache Member Functions~~~

// ~~getByte~~
//
// ~~~~~~~~~~~
char AssociativeCache::getByte(int addr, char input_bytes[4]){

    char output_to_cpu; // return variable from function

    int byte_no = addr&0x3; // gathering byte number (last 2 bits in addr) with "and" operation 
    int tag_no = (addr&0xFFFFFFFC)>>2; // gathering set number (30 bits after byte num in addr) using "and" and shift 2 bits to right (set_no>2)

    string print_tag; // string to store whether cache request was a hit or a miss for result printing

    // iterate through cache slots to check for cache hit
    for(int i = 0; i < block_size; i++){
        if(cache_entries[i].tag == tag_no && !cache_entries[i].invalid){
            // cache hit as tag in address matches tag within cache line from for loop

            print_tag = "hit"; // setting print_tag to hit
                                 // this is used when printing getByte request result
            output_to_cpu = cache_entries[i].bytes[byte_no]; // set output variable to requested byte
            hit_counter++; // increment hit counter to track this hit
            
            break; // break from loop as hit or miss condition has been met
        }
        else if (i == block_size - 1 && !cache_entries[i].invalid || cache_entries[i].invalid){
            // cache miss as address not found in all valid cache slots 
            // or cache is full thus need to overwrite line using FIFO basis

            print_tag = "miss"; // setting print_tag to miss   
                                  // this is used when printing getByte request result
            
            // as Associative cache entries act as a FIFO Queue as each cache entry must be pushed up before write
            // this is not an efficient method but is more representative of how hardware handles entries 
            for(int i = block_size - 1; i > 0; i--) // iterate through each cache slot
                cache_entries[i] = cache_entries[i-1]; // move up previous entry to current entry
            
            // append first cache slot with tag 
            cache_entries[0].tag = tag_no;
            // ensure inserted data into cache is valid for reading
            cache_entries[0].invalid = false;
            // copy bytes into address
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

        printf("Associative Cache Request Table\n"); // print title
        // print line seperator
        printf("+%.3s+%.13s+%.9s+%.11s+%.10s+%.4s+%.5s+\n",line.c_str(), line.c_str(), line.c_str(), line.c_str(), line.c_str(), line.c_str(), line.c_str(), line.c_str());
        // print title line of table
        printf("| # |Address (hex)|tag (hex)|byte# (hex)| hit/miss |hit#|miss#|\n"); 
        // print line sperator
        printf("+%.3s+%.13s+%.9s+%.11s+%.10s+%.4s+%.5s+\n",line.c_str(), line.c_str(), line.c_str(), line.c_str(), line.c_str(), line.c_str(), line.c_str(), line.c_str());
    }
    // print details regarding cache 
    printf("|%3d|   %08x  | %08x|     %1x     |Cache %-4s|%4d|%5d|\n",hit_counter + miss_counter, // total number of cache queries
                                                                                    addr, // address requested
                                                                                    tag_no, // address tag
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
void AssociativeCache::invalidateCache(){
    for(int i = 0; i < block_size; i++)
        cache_entries[i].invalid = true;
    return;
}