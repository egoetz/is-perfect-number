# is-perfect-number
A mutlithreaded C program to determine if the user's input is a perfect number.

# Command
To run the program, compile the C program and then execute it as follows:
```
./perfect 100 10
```
Where 100 is the potential perfect number that the process should test and ten is the number of threads the process should create.

# Timing
The effiency of most programs are improved by adding multithread capabilities. This program is no different. As we increase the number of threads by a power of two, the amount of time the program uses the CPU decreases by approximately a power of two. This suggests that there is a linear relation between number of threads and time to verify perfect number status. This pattern breaks down once the number of threads generated reaches the number of threads a given computer's CPU possesses. In the example times below, it is apparent that going from 8 to 16 threads produce a far smaller time decrease than going from 1 to 2 threads, 2 to 4 threads, and 4 to 8 threads each produce. This is unsuprising given that the output is from a computer with 6 CPU cores, each of which has 2 threads. Because of its CPU, this computer can only concurrently run 12 threads. Any decrease in time past that number (in this case a decrease of 0.383 seconds) is most likely caused by an increase in the likehood of a thread being scheduled 

In order to test the timing of this program, I ran the following commands:
```
time ./perfect 2305843008139952128 1
time ./perfect 2305843008139952128 2
time ./perfect 2305843008139952128 4
time ./perfect 2305843008139952128 8
time ./perfect 2305843008139952128 16
```

Each of these commands produced the following result:
```
The factors of 2305843008139952128 are 16, 144115188008747008, 32, 72057594004373504, 64, 36028797002186752, 
128, 18014398501093376, 256, 9007199250546688, 512, 4503599625273344, 1024, 2251799812636672, 2048, 
1125899906318336, 4096, 562949953159168, 8192, 281474976579584, 16384, 140737488289792, 1, 2, 
1152921504069976064, 32768, 70368744144896, 4, 576460752034988032, 65536, 35184372072448, 8, 288230376017494016,
131072, 17592186036224, 262144, 8796093018112, 524288, 4398046509056, 1048576, 2199023254528, 2097152,
1099511627264, 4194304, 549755813632, 8388608, 274877906816, 16777216, 137438953408, 33554432, 68719476704,
67108864, 34359738352, 134217728, 17179869176, 268435456, 8589934588, 536870912, 4294967294, 1073741824, 
and 2147483647.
2305843008139952128 is a perfect number.
```

This result returned in varying times according to how many threads were used. The details of timing and system usage are as follows:
```
./perfect 2305843008139952128 1  10.07s user 0.00s system 99% cpu 10.077 total
./perfect 2305843008139952128 2  10.22s user 0.00s system 199% cpu 5.111 total
./perfect 2305843008139952128 4  10.37s user 0.00s system 399% cpu 2.595 total
./perfect 2305843008139952128 8  12.53s user 0.00s system 736% cpu 1.702 total
./perfect 2305843008139952128 16  14.24s user 0.00s system 1079% cpu 1.319 total
```
