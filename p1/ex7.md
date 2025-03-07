# Exercise 7

The array size was $N = 100000000$

When calculating Fibonacci numbers for an array, using single or double precision directly impacts performance.

The following table shows the average execution times:

| Type | Time (s) |
| :--: | :------: |
| int  | 1.08     |
| float| 1.09     |
| double| 2.18    |

As we can see, using double precision takes twice as long to perform the calculations compared to single precision or integer types.
