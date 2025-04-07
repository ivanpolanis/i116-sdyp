# Exercise 3

When we parallelize the matrix multiplication by splitting the first loop (typically the row loop), the results are:


| N | Threads | Time |
|:-:|:-:|:-:|
|1000|2|1.604|
|1000|4|0.826|
|1000|8|0.563|
|1000|16|0.487|
|1000|32|0.451|

If we instead split the second loop (usually the column or inner loop), the performance results are:

| N | Threads | Time |
|:-:|:-:|:-:|
|1000|2|1.596|
|1000|4|0.834|
|1000|8|0.624|
|1000|16|0.607|
|1000|32|0.593|
 

As we can see, parallelizing the first loop scales better as the number of threads increases. This is likely because distributing work across rows avoids contention and improves memory locality, whereas parallelizing inner loops might cause overhead from thread management or cache inefficiencies.

# Exercise 4

The directive we need to use to correctly distribute the work between threads is `collapse(2)`. This tells OpenMP to distribute the combined iteration space of two nested for loops among the available threads.

# Exercise 5

When we parallelize each for loop using only `sections`, we limit the potential speedup to the number of sections. In this example, since we have only two sections, only two threads will be active—while the rest remain idle.

It is possible to parallelize the work inside each section to achieve greater speedup. However, if we restrict ourselves to using only sections, performance will not improve significantly.

