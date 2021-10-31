#include <fstream>
#include <vector>
#include <iostream>

#include "DMCache.h"
#include "AssociativeCache.h"
#include "TwoWayCache.h"

using namespace std;

// ~~~function predeclarations~~~

vector<unsigned int> loadAddresses(); // function to load address list from text file 

// ~~~functions~~~

// ~~main~~
//
// ~~~~~~~~
int main(){

    char input; // variable to store user input for cache selection
    
    cout << "Please enter one of the following to select a Cache to simulate:\n"; // prompting user input with printout
    cout << "\t1 - Associative Cache\n\t2 - Direct Mapped Cache\n\t3 - 2 Way Set Associative Cache\n";

    do{ // repeatedly request user input until a value from 1 to 3 has been entered 
        cout << "Enter value from 1-3:"; // printout prompting user input
        cin >> input; // recieving the user input
    }while(input != '1' && input != '2' && input != '3'); // checking if user has entered a valid input

    vector<unsigned int> addr = loadAddresses(); // loading list of addresses into addr vector
                                                 // these will be used when emulating the cache 

    char input_bytes[4] = {'b','y','t','e'}; // sample bytes to use to emulate bytes being transfered from DRAM
    
    if(input =='1'){
        unsigned int cache_size = 0; //variable to store user input for associative cache block size
        
        cout << "Please specify a value for the block size of the associative cache:"; // prompting user input
        
        // loop until a valid integer input has been recieved 
        while(!(cin >> cache_size)){ // recieve and check if input is valid
            cout << "\nError, please input an integer value:"; // generate an error output
            
            cin.clear(); // clear the previous input
            cin.ignore(1000,'\n'); // discard the previous input before taking another input
        }
        
        AssociativeCache cache_obj(cache_size);  // create Associative Cache Object with user specified integer input
        
        // loop to emulate all address requests from the addr vector
        for(int i = 0; i < addr.size(); i++) // iterate through each address in the address request vector
            cache_obj.getByte(addr[i], input_bytes); // emulate a Cache Read Request using the cache object
    }
    else if(input== '2'){
        DMCache cache_obj; // create Direct Mapped Cache Object
        
        // loop to emulate all address requests from the addr vector
        for(int i = 0; i < addr.size(); i++) // iterate through each address in the address request vector
            cache_obj.getByte(addr[i], input_bytes); // emulate a Cache Read Request using the cache object
    }
    else{
        TwoWayCache cache_obj; // create Two Way Set Associative Cache Object

        // loop to emulate all address requests from the addr vector
        for(int i = 0; i < addr.size(); i++) // iterate through each address in the address request vector
            cache_obj.getByte(addr[i], input_bytes); // emulate a Cache Read Request using the cache object
    }
    
    return 0;
}

// ~~loadAddresses~~
//
// ~~~~~~~~~~~~~~~~~
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