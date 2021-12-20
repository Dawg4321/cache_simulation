# CPU Cache Simulator

This is a simple program which simulates read operation on a a series of CPU caches designs. This was completed as apart of the Computer Architecture 3 Module for college. The following 3 types of cache have been simulated in this implementation:

- Fully Associative Cache

- Direct Mapped Cache

- 2-Way Set Associative Cache

## Compilation

Code is built around Linux though should work fine with Windows. To compile this code with g++, run the following command with all of the code files in the working directory:

```g++ main.cpp TwoWayCache.cpp AssociativeCache.cpp DMCache.cpp -o myprog```

Once compiled, the simulator can then be run by using the following command in the directory of the compiled file:

```./myprog```
