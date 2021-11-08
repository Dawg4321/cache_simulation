#include <stdio.h>
#include <stdbool.h>


struct FetchCache{
    bool LRU;
    bool invalid;
    unsigned short upper_tag; 
    char bytes[4];
};

char DirectMap(int x, struct FetchCache direct_m[], bool *cache_hit);

int main(){
    struct FetchCache direct_m[64384];

    // initialise all values in cache to invalid
    for(int i =0; i < 64384; i++){
        direct_m[i].invalid = true;
    }   

    int x = 0xA000AB01;
    bool direct_cache_hit = false;
    char h = DirectMap(x, direct_m, &direct_cache_hit);
    h = DirectMap(x, direct_m, &direct_cache_hit);
    x = 0xA000AB02;
    h = DirectMap(x, direct_m, &direct_cache_hit);
}

char DirectMap(int x, struct FetchCache direct_m[], bool *cache_hit){

    char output_to_cpu; // return variable 
    int byte_no = x&0x3; // and last 2 bits 
    int set_no = (x&0x0000FFFC)>>2; // and 14 bits and shift 2 bits to right (set_no>2)
    int upper_tag_no = (x&0xFFFF0000)>>16; // and 16 bits

    char input_bytes[4] = {'r','y','a','n'}; // sample bytes for addresses

    if(direct_m[set_no].upper_tag == upper_tag_no && !direct_m[set_no].invalid){
        // cache hit
        output_to_cpu = direct_m[set_no].bytes[byte_no]; // set output variable to corresponding byte
        printf("cache hit\n");

        // returning cache hit to main
        *cache_hit = true;
    }
    else{
        // cache miss

        // set address to corresponding values
        direct_m[set_no].upper_tag = upper_tag_no;
        direct_m[set_no].invalid = false;

        // copy bytes into address
        for(int i = 0; i < 4; i++)
            direct_m[set_no].bytes[i] = input_bytes[i];   
        printf("cache miss\n");
        
        // returning cache miss to main
        *cache_hit = false;
    }
    return output_to_cpu;
}
// TODO: Implement 2 way set associative
// 2 way set associative is the same except 2 struct arrays and search through both arrays for hit