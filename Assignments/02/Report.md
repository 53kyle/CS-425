# Results for Assignment 02

## Improvement Iterations

Here's a table showing the improvements I did to make the application go faster.  The **Time** column in the table represents the _wall-clock time_ for a program run.

| Version | Time (test/final) | Speedup (test/final) | Memory (KB) (test/final) | Changes |
| :-----: | ---- | :-----: | :------: | ------- |
| [0](lychrel0.cpp) | 108.06s/- | &mdash; | 3676/- | Baseline |
| [0](lychrel0.cpp) | 3.2s/643.31s | 33.8x/- | 3656/4464 | Baseline w/03 optimization |
| [1](lychrel1.cpp) | 13.04s/- | 8.2x/- | 4028/- | Threaded |
| [1.1](lychrel1.cpp) | 0.52s/95.17s | 207.8x/x | 3992/4796 | Threaded w/03 optimization |
| [2](lychrel2.cpp) | 8.38s/- | 12.9x/- | 4264/- | Scheduling (single value) |
| [2.1](lychrel2.cpp) | 0.47s/117.09s | 229.9x/x | 4264/36636 | Scheduling (single value) w/03 optimization |
| [3](lychrel3.cpp) | 8.57s/- | 12.6x/- | 4224/- | Scheduling (multiple value) |
| [3.1](lychrel3.cpp) | 0.41s/117.36s | 263.6x/x | 4184/36648 | Scheduling (multiple value) w/03 optimization |
| [4](lychrel4.cpp) | 8.54s/- | 12.7x/- | 4208/- | Atomic maxIter |
| [4.1](lychrel4.cpp) | 0.34s/94.46s | 317.8x/x | 4180/36644 | Atomic maxIter w/03 optimization |
| [4.2](lychrel4.cpp) | 0.30s/88.6s | 360.2x/x | 4232/36644 | Atomic maxIter w/03 optimization & march=native flag |
| [5](lychrel5.cpp) | 0.20s/85.40s | 540.3x/x | 4312/36872 | Loop unroll |
| [6](lychrel6.cpp) | 0.35s/46.97s | 308.7x/x | 4204/38120 | No loop unroll, added barrier and mutex, & removed Number.reverse() calls |


## Profiling Analysis

### Notes
Versions 5 and 6 are both compiled with O3 and march=native.

### Baseline (0)
Without any optimizations, this program takes a LONG time, especially with the final values. I belive this is because, for 50000 iterationss, there are between 1 and 7500 digit-wise additions, 1-7500 reverse-copies of vectors (some of which are very large), and 1-7500comparison algorithms to check if a Number object is palindomic. That's a lot of math.

### Threaded (1)
All I did in this version is split the overall work post-reading the file into 10 threads. This made a huge difference, with a roughly 8x speedup.

### Scheduling (single value) and scheduling (multiple values) (2 and 3)
In versions 2 and 3, I attempted to even out the load that each thread was taking on by retrieving values via LychrelData's getNext() functions. While this didn't make much of a difference (actually it ran slightly slower), I'm sure it was beneficial to do so.

### Atomic maxIter (4)
Prior to making this modification, my method for maxIter was to split maxIter into an array of 10 values and records into 10 separate vectors, then at the end of the program, iterate through them to find the maximum. To my surprise, changing this made a massive difference in execution time. 

### Loop unroll (5)
In this version, all I did was attempt to reduce the number of comparisons the largest in the main function would make by 8x. However, this hardly made a difference, which I should've expected considering those comparisons don't take very long relative to everything else.

### No loop unroll, added barrier and mutex, & removed Number.reverse() calls (5)
This version, by far, made the biggest difference in execution time. The first thing I did here was undo everything I did in version 5, since I didn't think it made enough of a difference (and actually made a negative impact on my own, faster machine). Secondly, I added a barrier and mutex to protect against data race, which I noticed did cause incorrect output every so often. Finally, I noticed that Number 'r' wasn't actually doing anything in summing the regular and reverse numbers, so I removed it. That's where the majority of the time savings here came from.

### Using -O3
I can't explain why, but -O3 actually runs significantly faster than -Ofast on blue. I'm talking -Ofast is over 20 seconds slower. On my own machine, this isn't the case, but the difference between the two is negligible.

### Using -march=native
This also makes a decent difference on blue and hardly any difference on my own machine. I'm guessing it's due to a lack of optimization for apple silicon, and/or the lack of certain registers in apple silicon (I know AVX registers have no equivalent).
