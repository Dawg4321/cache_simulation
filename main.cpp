#include <stdio.h>
#include <fstream>
#include <vector>

#include "DMCache.h"
#include "AssociativeCache.h"

using namespace std;

vector<unsigned int> loadAddresses(){
    ifstream addr_file; // file stream object to load files from addr.txt
    
    vector<unsigned int> addr_arr; // vector to store addresses in as unsigned ints

    string addr_buff = ""; // buffer for storing address from txt file
    unsigned int addr_int; // buffer for storing address in after converting from hex string value
    
    addr_file.open("addr.txt",ios::in); // open addr.t for reading

    if (addr_file.is_open()){  // checking if addr.txt is open
        while(getline(addr_file, addr_buff)){  // read line by line from addr.txt and place into string
            addr_int = (int)strtol(addr_buff.c_str(), NULL, 16); // convert hex value within string to unsigned int 
            addr_arr.push_back(addr_int); // adding hex value to vector
        }
    }
    addr_file.close(); // close addr.txt
    
    return addr_arr; // return address vector
}

int main(){

}

// TODO: Implement 2 way set associative
// 2 way set associative is the same except 2 struct arrays and search through both arrays for hit