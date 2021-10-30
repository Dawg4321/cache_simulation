#include <fstream>
#include <vector>
#include <iostream>

#include "DMCache.h"
#include "AssociativeCache.h"
#include "TwoWayCache.h"

using namespace std;

vector<unsigned int> loadAddresses();

int main(){
    char input;

    cout << "Please enter one of the following to select a Cache to simulate:\n";
    cout << "\t1 - Associative Cache\n\t2 - Direct Mapped Cache\n\t3 - 2 Way Set Associative Cache\n";
    do{
        cout << "Enter value from 1-3:";
        cin >> input;
    }while(input != '1' && input != '2' && input != '3');

    vector<unsigned int> addr = loadAddresses();
    char output;
    char input_bytes[4] = {'b','y','t','e'}; // sample bytes for addresses
    
    if(input =='1'){
        AssociativeCache cache_obj(10);
        
        for(int i = 0; i < addr.size(); i++)
            output = cache_obj.getByte(addr[i], input_bytes);
    }
    else if(input== '2'){
        DMCache cache_obj;
        
        for(int i = 0; i < addr.size(); i++)
            output = cache_obj.getByte(addr[i], input_bytes);
    }
    else{
        TwoWayCache cache_obj;

        for(int i = 0; i < addr.size(); i++)
            output = cache_obj.getByte(addr[i], input_bytes);
    }
    
    return 0;
}

vector<unsigned int> loadAddresses(){
        
    vector<unsigned int> addr_arr; // vector to store addresses in as unsigned ints. this is the return variable

    unsigned int addr_int; // buffer for storing addresses from filestream

    ifstream addr_file; // file stream object to load files from addr.txt
    addr_file.open("addr.txt",ios::in); // open addr.txt for reading

    if (addr_file.is_open()){  // checking if addr.txt is open
    }
    while(!addr_file.eof()){  // reads data from addr.txt and place into string
        addr_file >> hex >> addr_int; // convert hex value within string to unsigned int 
        addr_arr.push_back(addr_int); // adding hex value to vector
    }
    addr_file.close(); // close addr.txt
    
    return addr_arr; // return address vector
}