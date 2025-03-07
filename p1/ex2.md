# Exercise 2

After testing fibonacci function we've got the following table:

|  N  | Recursive | Iterative |
| :-: | :-------: | :-------: |
| 10  |  214  ns  |     0  s  |
| 20  |  22.7 us  |  92.4  ns |
| 30  |  2.19 ms  |  95.4  ns |
| 40  |  268  ms  |  95.4  ns |
| 50  |  33.1  s  |  119.2 ns |


As we can observe, the recursive method takes significantly longer to execute compared to the iterative method, especially as $N$ increases.

This happens because the recursive version has a time complexity of $O(2^n)$, due to the exponential growth of recursive calls. Each call generates two more calls, which leads to an explosion of branches.

On the other hand, the iterative version has a time complexity of $O(n)$, since it computes the result in a linear loop without making additional recursive calls. This makes the iterative approach much more efficient for large values of $N$.
