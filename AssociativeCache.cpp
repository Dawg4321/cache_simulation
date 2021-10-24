#include "AssociativeCache.h"

AssociativeCache::AssociativeCache(int size){
    cache_block1 = new AssociativeCacheLine[size];
    block_size = size;
    fifo_tracker = 0;
    invalidateCache();
}

AssociativeCache::~AssociativeCache(){
    delete[] cache_block1;
}


// ~~~ AssociativeCache::GetByte ~~~
//
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
char AssociativeCache::getByte(int addr, bool *cache_hit, char input_bytes[4]){

    char output_to_cpu; // return variable from function

    int byte_no = addr&0x3; // gathering byte number (last 2 bits in addr) with "and" operation 
    int tag_no = (addr&0xFFFFFFFC)>>2; // gathering set number (30 bits after byte num in addr) using "and" and shift 2 bits to right (set_no>2)

    for(int i = 0; i < block_size; i++){
        if (cache_block1[i].invalid){
            // cache miss as address not found in all valid cache slots
            miss_counter++;

            loadCacheSlot(i, tag_no, input_bytes);
            
            printf("%d. cache miss on 0x%x\n", miss_counter,addr);
            printf("\tByte No: %d, Contents = %c\n", byte_no, input_bytes[byte_no]);
            
            // returning cache miss to main
            *cache_hit = false;

            // set output variable to requested nbyte
            output_to_cpu = cache_block1[i].bytes[byte_no];
            break;
            
        }
        else if(cache_block1[i].tag == tag_no){
            // cache hit
            hit_counter++;

            printf("%d. cache miss on Addr: 0x%x\n", hit_counter,addr);
            printf("\tByte No: %d, Contents: %c\n", byte_no, input_bytes[byte_no]);

            // returning cache hit to main
            *cache_hit = true;

            output_to_cpu = cache_block1[i].bytes[byte_no]; // set output variable to requested byte
            break;
        }
        else if (i == block_size - 1 && !cache_block1[i].invalid){
            // cache miss as address not found in all valid cache slots
            // cache is currently full thus need to overwrite line using FIFO basis
            miss_counter++;
            
            printf("%d. cache miss on 0x%x\n", miss_counter,addr);
            printf("\tByte No: %d, Contents = %c\n", byte_no, input_bytes[byte_no]);

            loadCacheSlot(fifo_tracker, tag_no, input_bytes); // loading byte into cache

            // determining which cache line has been in the cache the longest
            // line which has been in cache longest is the slot after the previously longest cache line 
            // thus fifo_tracker must be incremented
            if(fifo_tracker != block_size - 1) // ensuring the value of fifo_tracker doesn't increment to an invalid address
                fifo_tracker++;
            else // if the last slot of the cache has been met, pointer to the start of the cache
                fifo_tracker = 0;

            break;
        }
        else{
            // do nothing as current slot in cache is valid but does not have a matching tag
        }
    }
    return output_to_cpu;
}

void AssociativeCache::loadCacheSlot(int cache_slot, int tag, char bytes[4]){
    // append current cache slot with tag 
    cache_block1[cache_slot].tag = tag;
    // ensure cache is valid for reading
    cache_block1[cache_slot].invalid = false;
    // copy bytes into address
    for(int j = 0; j < 4; j++)
        cache_block1[cache_slot].bytes[j] = bytes[j]; 
    return;
}

void AssociativeCache::invalidateCache(){
    for(int i = 0; i < block_size; i++)
        cache_block1[i].invalid = true;
    
    return;
}