# Results for Assignment 01

## Improvement Iterations

Here's a table showing the improvements I did to make the application go faster.  The **Time** column in the table represents the _wall-clock time_ for a program run.

| Version | Time | Speedup | Memory (KB) | Changes |
| :-----: | ---- | :-----: | :------: | ------- |
| [01](01.cpp) | 9.96s | &mdash; | 1041324 | Initial version - no changes |
| [02](02.cpp) | 5.66s | 1.76x | 2775480 | Threaded Implementation |
| [03](03.cpp) | 6.19s | 1.61x | 2778504 | AVX Register + Threaded Implementation |
| [04](04.cpp) | 12.54s | 0.79x | 1040888 | Further AVX Register Optimization, No Threads |
| [05](05.cpp) | 6.10s | 1.63x | 2780844 | Second Attempt w/ AVX Register + Threaded Implementation |
| [01.5](01.cpp) | 1.86s | 4.89x | 1041320 | Initial version with -Ofast |
| [02.5](02.cpp) | 2.47s | 4.03x | 1909424 | Threaded Implementation with -Ofast |
| [03.5](03.cpp) | 2.91s | 2.00x | 2691668 | AVX Register + Threaded Implementation with -Ofast |
| [04.5](04.cpp) | 2.56s | 1.12x | 2787564 | Further AVX Register Optimization, No Threads and -Ofast|
| [05.5](05.cpp) | 2.8s | 1.12x | 2787564 | Second Attempt w/ AVX Register + Threaded Implementation and -Ofast |

## Profiling Analysis

### Notes
I compiled all versions with flags -mavx2 -mavx512f in order to make sure avx registers will be used

### Initial Review (01)
In version 01, unlike in the sample results, dot() was taking up the largest amount of the program's execution time, with perspectiveDivide() not far behind. I decided to take a different approach from what we've seen in class, using threads as I was taught in CS 450 (operating systems).

### Threaded Implementation (02)
In CS 450, we used four threads to make a program that calculates pi run roughly 4x faster. In this case, I used four threads to calculate the smallest triangle for 1/4 of the total faces per thread, then once all four threads were done computing, I picked out the smallest of only four triangles. I figured this would be a good solution because it didn't seem like it would be possible to cut down on how many times perspectiveDivide was being called, and I don't think turning the division into multiplication will help. Ultimately, I got a less than 2x speedup, which I wasn't too impressed with.

For some reason, when I compile this version with -Ofast, the output is incorrect.

### AVX Register + Threaded Implementation (03)
In class, AVX registers were introduced to us as a way to compute multiple values in one instruction. In this case, since perspectiveDivide() and dot() are both taking up a lot of time, I thought I'd try rewriting both functions to use AVX registers. However, this actually made the program run slower. My best guess as to why it would run slower, is because when compiling with -mavx512f, the compiler already utilizes AVX registers whenever possible, and maybe my own implementation is less efficient than what the compiler would've done automatically.

### Further AVX Register Optimization, No Threads (04)
On monday, we learned that it might be important to make sure that the memory for AVX registers are aligned to 64 bytes, so that's one change I made here. I also rewrote a lot more code to utilize AVX registers, including transforming four separate calls of the dot() function to one. Because I removed threads in 04, it seems like it runs a lot slower than 03, but in 03 without threads, it would've run in a whopping 24 seconds compared to 12 with version 04. This is still obviously slower than version 01, which is disappointing.

### Second Attempt w/ AVX Register + Threaded Implementation (05)
For this version, all I did was take all of the AVX code from 04 and added back in the threads, and it isn't much if any faster than 03. My best guess as to why this is, is because of the overhead included in spinning up threads, like maybe that's taking up enough time to nullify any other improvements I may have made. Or, I need to learn how to better utilize threads and/or AVX registers.

### Using -Ofast
I found that -Ofast (as the name would imply) was always fastest for all implementations that I have. However, it is leading to incorrect output in some cases (particularly the ones with multiple threads involved), which I don't understand.
