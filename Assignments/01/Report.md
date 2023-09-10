# Results for Assignment 01

## Improvement Iterations

Here's a table showing the improvements I did to make the application go faster.  The **Time** column in the table represents the _wall-clock time_ for a program run.

| Version | Time | Speedup | Memory (KB) | Changes |
| :-----: | ---- | :-----: | :------: | ------- |
| [01](01.cpp) | 9.46s | &mdash; | 1043192 | Initial version - no changes |

## Profiling Analysis

### Initial Review (01)
In version 01, just like in the sample results, perspectiveDivide() was indeed taking up around 50% of the program's execution time. I decided to take a different approach, using threads as I was taught in CS 450 (operating systems).

### Threaded Implementation (02)
In CS 450, we used four threads to make a program that calculates pi run roughly 4x faster. In this case, I used four threads to calculate the smallest triangle for 1/4 of the total faces per thread, then once all four threads were done computing, I picked out the smallest of only four triangles. I figured this would be a good solution because it didn't seem like it would be possible to cut down on how many times perspectiveDivide was being called, and I don't think turning the division into multiplication will help. Ultimately, I got a less than 2x speedup, which isn't 4x but still isn't terrible.

Looking at the profile, I did get perspectiveDivide() down to only 15% of the program's execution time, but both perspectiveDivide() and dot() are still up at the top. Something I don't undertand is that now I actually have a lot more calls to both functions. I might be misunderstanding something about how the code works, but since I'm using pointers to the vertices and faces, and I can confirm that I am only computing 1/4 of them in each thread, I'm stumped as to why this would be.

### Vector Register Implementation (03)
In class, vector registers were introduced to us as a way to compute multiple values in one instruction. In this case, since perspectiveDivide() and dot() are both taking up a lot of time, I thought I'd try reimplementing both functions to use vector registers. However, this actually made the program run significantly slower, nearly taking as much time as the initial version, which is pretty bad since it's still threaded. Using these functions without threads actually made the program run over 2x slower compared to the initial version. 

I suspect the reason this is so slow is because I'm allocating memory in each call of perspectiveDivide() and dot(), which wasn't necessary before. I'll look into solving this as well.
