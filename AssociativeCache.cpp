#include "AssociativeCache.h"

AssociativeCache::AssociativeCache(int size){
    cache_entries = new AssociativeCacheLine[size];
    block_size = size;
    invalidateCache();
}

AssociativeCache::~AssociativeCache(){
    delete[] cache_entries;
}


// ~~~ AssociativeCache::GetByte ~~~
//
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
char AssociativeCache::getByte(int addr, char input_bytes[4]){

    char output_to_cpu; // return variable from function

    int byte_no = addr&0x3; // gathering byte number (last 2 bits in addr) with "and" operation 
    int tag_no = (addr&0xFFFFFFFC)>>2; // gathering set number (30 bits after byte num in addr) using "and" and shift 2 bits to right (set_no>2)

    // iterate through cache slots to check for cache hit
    for(int i = 0; i < block_size; i++){
        if(cache_entries[i].tag == tag_no){
            // cache hit
            hit_counter++;

            printf("%d. cache hit on Addr: 0x%x\n", hit_counter,addr);
            printf("\tByte No: %d, Contents: %c\n", byte_no, input_bytes[byte_no]);

            output_to_cpu = cache_entries[i].bytes[byte_no]; // set output variable to requested byte
            break;
        }
        else if (i == block_size - 1 && !cache_entries[i].invalid || cache_entries[i].invalid){
            // cache miss as address not found in all valid cache slots
            // cache is currently full thus need to overwrite line using FIFO basis
            miss_counter++;
            
            printf("%d. cache miss on 0x%x\n", miss_counter,addr);
            printf("\tByte No: %d, Contents = %c\n", byte_no, input_bytes[byte_no]);

            // push up each entry into the queue
            // acts as a FIFO Queue as first value in will be pushed out first 
            for(int i = block_size - 1; i > 0; i--)
                cache_entries[i] = cache_entries[i-1];
            
            // append first cache slot with tag 
            cache_entries[0].tag = tag_no;
            // ensure inserted data into cache is valid for reading
            cache_entries[0].invalid = false;
            // copy bytes into address
            for(int j = 0; j < 4; j++)
                cache_entries[0].bytes[j] = input_bytes[j]; 

            break; // break from loop as cache miss condition is true. Not possible to achieve a hit
        }
        else{
            // do nothing as current slot in cache is valid but does not have a matching tag
        }
    }
    return output_to_cpu;
}

void AssociativeCache::invalidateCache(){
    for(int i = 0; i < block_size; i++)
        cache_entries[i].invalid = true;
    return;
}