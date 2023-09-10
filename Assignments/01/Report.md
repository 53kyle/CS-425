# Results for Assignment 01

## Improvement Iterations

Here's a table showing the improvements I did to make the application go faster.  The **Time** column in the table represents the _wall-clock time_ for a program run.

| Version | Time | Speedup | Memory (KB) | Changes |
| :-----: | ---- | :-----: | :------: | ------- |
| [01](01.cpp) | 9.96s | &mdash; | 1041324 | Initial version - no changes |
| [02](02.cpp) | 5.27s | 1.89x | 2775480 | Threaded Implementation |
| [03](03.cpp) | 8.90s | 1.12x | 2787564 | Vector Register + Threaded Implementation |
| [01.5](01.cpp) | 2.04s | 4.89x | 1041320 | Initial version with -Ofast |
| [02.5](02.cpp) | 2.47s | 4.03x | 1909424 | Threaded Implementation with -Ofast |
| [03.5](03.cpp) | 4.96s | 2.00x | 2691668 | Vector Register + Threaded Implementation with -Ofast |

## Profiling Analysis

### Initial Review (01)
In version 01, unlike in the sample results, dot() was taking up the largest amount of the program's execution time, with perspectiveDivide() not far behind. I decided to take a different approach, using threads as I was taught in CS 450 (operating systems).

### Threaded Implementation (02)
In CS 450, we used four threads to make a program that calculates pi run roughly 4x faster. In this case, I used four threads to calculate the smallest triangle for 1/4 of the total faces per thread, then once all four threads were done computing, I picked out the smallest of only four triangles. I figured this would be a good solution because it didn't seem like it would be possible to cut down on how many times perspectiveDivide was being called, and I don't think turning the division into multiplication will help. Ultimately, I got a less than 2x speedup, which isn't 4x but still isn't terrible.

Looking at the profile, I did get perspectiveDivide() down to only 15% of the program's execution time, but both perspectiveDivide() and dot() are still up at the top. Also, I'm seeing that the number of times perspectiveDivide() is being called is actually significantly less, which I honestly can't explain because nothing I changed should've caused it to be called less. Maybe profiling is inaccurate when using multiple threads?

### Vector Register + Threaded Implementation (03)
In class, vector registers were introduced to us as a way to compute multiple values in one instruction. In this case, since perspectiveDivide() and dot() are both taking up a lot of time, I thought I'd try reimplementing both functions to use vector registers. However, this actually made the program run significantly slower, nearly taking as much time as the initial version, which is pretty bad since it's still threaded. Using these functions without threads actually made the program run over 2x slower compared to the initial version. 

I suspect the reason this is so slow is because I'm allocating memory in each call of perspectiveDivide() and dot(), which wasn't necessary before. I'll look into solving this as well.

### Using -Ofast
I found that -Ofast (as the name would imply) was always fastest for all implementations that I have. However, I find it odd that while implementations 02 and 03 are both faster than 01 by default, they're slower when using this optimization flag.
