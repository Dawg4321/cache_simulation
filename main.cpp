#include <fstream>
#include <vector>
#include <iostream>

#include "DMCache.h"
#include "AssociativeCache.h"
#include "TwoWayCache.h"

using namespace std;

// function forward declarations 
vector<unsigned int> loadAddresses();

/* ~~~~~~~~~~~~~~~~~~~~main~~~~~~~~~~~~~~~~~~~~

    Allows for the user to simulate a series of different CPU caches using address values from a txt file.

    ~~ Operation ~~

    The user is first prompted to enter a value from 1 to 3.
    This input value selects which type of cache will be simulated. 1 = Associative Cache, 2 = Direct Mapped Cache, 3 = 2 Way Set Associative Cache.
    Once a valid user input has been recieved, a series of hexidecimal addresses are then loaded from addr.txt into a vector. 
    These address will be passed into the cache in the order of occurance within the text file.
    Now that all the required information about the cache has been selected, the specifications of the chosen cache are printed.
    This is followed by a table listing the result of each cache request. 

    ~~ Compiling and Running ~~
    
    To compile on Unix systems with g++, use the following:

    g++ main.cpp TwoWayCache.cpp AssociativeCache.cpp DMCache.cpp -o cache_sim

    To run, use the following in the directory of compilation:

    ./cache_sim

   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
int main(){

    // printing text specifying which cache models can be simulated
    cout << "Please enter one of the following to select a Cache to simulate:\n";
    cout << "\t1 - Associative Cache\n\t2 - Direct Mapped Cache\n\t3 - 2 Way Set Associative Cache\n";
    
    string input; // variable to hold input from chin

    // repeatly ask user for input using cin until input is valid
    // minimal error correction is done as purpose of project is to simulate cache
    do{
        cout << "Enter value from 1-3:"; // prompting user to enter a value between 1 and 3
                                         // this is to help determine which cache to simulate
        cin >> input; // reading input from keyboard

    }while(input != "1" && input != "2" && input != "3"); // asking for new input if input value is invalid

    vector<unsigned int> addr = loadAddresses(); // loading addresses to pass into cache from addr.txt

    // declaring variables before simulation
    char input_bytes[4] = {'b','y','t','e'}; // getByte requires some form of bytes to be passed into it so data can be loaded into cache
                                             // thus these bytes will be used in place of each request
    
    // if statement checks and simulate specific cache based on user input
    if(input == "1"){ // if user selects associative cache 
        unsigned int cache_size = 0; //variable to store user input for associative cache line number
        
        cout << "Please specify a value for the number of lines in the associative cache: "; // prompting user input
        
        // loop until a valid integer input has been recieved 
        while(!(cin >> cache_size)){ // recieve and check if input is valid
            cout << "\nError, please input an integer value: "; // generate an error output
            
            cin.clear(); // clear the previous input
            cin.ignore(1000,'\n'); // discard the previous input before taking another input
        }

        AssociativeCache cache_obj(cache_size);  // create Associative Cache Object with user specified integer input
        
        cache_obj.printSpecs(); // printing specifications of cache being modelled

        // loop to emulate all address requests from the addr vector
        for(int i = 0; i < addr.size(); i++) // iterate through each address in the address request vector
            cache_obj.getByte(addr[i], input_bytes); // emulate a Cache Read Request using the cache object
    }
    else if(input == "2"){ // if user selects direct mapped cache 

        DMCache cache_obj; // create Direct Mapped Cache Object
        
        cache_obj.printSpecs(); // printing specifications of cache being modelled

        // loop to emulate all address requests from the addr vector
        for(int i = 0; i < addr.size(); i++) // iterate through each address in the address request vector
            cache_obj.getByte(addr[i], input_bytes); // emulate a Cache Read Request using the cache object
    }
    else{// if user selects two way set associative cache 

        TwoWayCache cache_obj; // create Two Way Set Associative Cache Object

        cache_obj.printSpecs(); // printing specifications of cache being modelled

        // loop to emulate all address requests from the addr vector
        for(int i = 0; i < addr.size(); i++) // iterate through each address in the address request vector
            cache_obj.getByte(addr[i], input_bytes); // emulate a Cache Read Request using the cache object
    }
    
    return 0;
}

/* ~~~~~~~~~~~~~~~~~~~~loadAddresses~~~~~~~~~~~~~~~~~~~~

    Loads a list of addresses from a file into an unsigned int vector.

    ~~ Operation ~~

    Function first opens up a txt file called "addr.txt" using an ifstream object. 
    If addr.txt fails to open, returns an empty vector and outputs an error msg.
    While loop is used to read each string of hex addresses and loads them into the addr_arr vector.

    ~~ output variables ~~

    -> vector<unsigned int> addr_arr = vector pass loaded addresses back to previous scope 

   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
vector<unsigned int> loadAddresses(){
        
    vector<unsigned int> addr_arr; // vector to store addresses in as unsigned ints. this is the return variable

    unsigned int addr_int; // buffer for storing addresses from filestream

    ifstream addr_file; // file stream object to load files from addr.txt
    addr_file.open("addr.txt",ios::in); // open addr.txt for reading

    if (!addr_file.is_open()){  // checking if addr.txt is open
        cout << "Error opening address text file. Please ensure the address file is called 'addr.txt'\n"; // print error message
        return addr_arr; // return from function with empty vector as addresses can't be loaded
    }
    
    // reads data from addr.txt and place into string
    while(addr_file >> hex >> addr_int){ // load file from addr.txt to unsigned int
                                         // >> hex tells that the value being loaded is hex 
        addr_arr.push_back(addr_int); // adding loaded hex value to vector
    }
    addr_file.close(); // close addr.txt
    
    return addr_arr; // return address vector
}