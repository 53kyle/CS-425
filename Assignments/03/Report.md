# Results for Assignment 03

## Improvement Iterations

Here's a table showing the improvements I did to make the application go faster.  The **Time** column in the table represents the _wall-clock time_ for a program run.

| Version | Time | Speedup | Changes |
| :-----: | ---- | :-----: | ------- |
| [01](01.cpp) | 208s | &mdash; | Baseline |
| [02](02.cpp) | 234s | 0.88x | Simple asynchronous task |
| [03](03.cpp) | 222s | 0.93x | Packaged task |
| [04](04.cpp) | 114s | 1.82x | Queued tasks |
| [05](05.cpp) | 109s | 1.91x | More queued tasks |


## Profiling Analysis

### Notes
- I expect the speedup would be much greater on any internet connection besides my own. My internet's download speed maxes out at 50 megabits/sec, which is only slightly higher than I was measuring consistently with version 05. There might be a bottleneck with SSU's internet upload speed as well.

### Baseline (01)
Running this initial version with the "network" tab open, I could immediately see that for every request of an image, the wait time was very high. In fact, for most images, the wait time was significantly higher than the time spent actually downloading the image. So, if I want to optimize this, I'll want to reduce that wait time. In this baseline version, wait times were between 2-5 seconds per image.

### Simple asynchronous task (02)
For this version, all I did was wrap the inside of the "while(connection)" loop in a task and immediately launch it asynchronously. This made the runtime significantly slower. I'm guessing that this is because the server still had to respond to requests in a sequential manner, one after another, and all we pushed off to other threads was the work of...waiting on data. Also, there's now the extra work being done to launch tasks, which doesn't help matters.

### Packaged task (03)
For this version, to be honest, it was 3AM and I was tired, so I went entirely in the wrong direction. I thought it would be a good idea to build directly off of version 02, but make a packaged task that does just the work of creating an HTTPRequest and getting an HTTPResponse from the recieved message, which predictably doesn't do anything different from version 02 besides add complexity. I think the logic behind it in my head was that if I could package the whole inside of the loop, I could create a queue of that task, but I realized pretty quickly that trying to move Sessions around wasn't going to happen. And ultimately, none of that would've improved runtime since I'd still be queuing up tasks sequentially as requests came in. 

### Queued tasks (04)
For this version, I started off with a fresh slate, and it dawned on me that if I had multiple threads, or at least asynchronous tasks, simultaneously listening in for requests, I could properly parallelize this program. So, instead of wrapping the inside of the loop in a task, I wrapped the outside of the loop as well, then queued up and launched 10 (thinking 10 threads) instances of that task asynchronously. For the first time, I got a reduction in wait times! In fact, with this version, they're as low as I imagine they'll go, down from 2-5 seconds to 50 milliseconds. The one thing I'd improve on (and did with version 05) is simplicity. For instance, for whatever reason, I thought it would be a good idea to have an asynchronous "polling" function, which would listen in on the queue of tasks and run tasks as soon as they were queued up. But since I was only running these tasks upon first running the server, this was unneccesary.

### More queued tasks (05)
For this version, I cleaned up my code, and now instead of running 10 tasks, I'm running a whopping 300. It's probably overkill and might even be bad practice, but I figured since the system manages how asynchronous tasks run, and since we have a theoretical 300 images to retrieve, I might as well throw a ton of tasks at the problem. Runtime was a bit faster than version 04, so I don't think I made things worse here at the very least.
