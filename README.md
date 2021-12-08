# CPU Cache Simulator

This is a simple program which simulates read operation on a a series of CPU caches designs. This was completed as apart of the Computer Architecture 3 Module for college. The following 3 types of cache have been simulated in this implementation:

- Fully Associative Cache

- Direct Mapped Cache

- 2-Way Set Associative Cache

## Compilation

Code is built around Linux though should work fine with Windows. To compile this code with g++, run the following commands with all of the code files in the working directory:

1 - g++ main.cpp TwoWayCache.cpp AssociativeCache.cpp DMCache.cpp -o myprog

2 - ./myprog
