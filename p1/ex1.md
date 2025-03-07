# Exercise 1

## A

### i

This was tested with $N=1024$.

Execution time with functions `getValor()` and `setValor()`:
    13.655s

```c
for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      setValor(C, i, j, ORDENXFILAS, 0);
      for (k = 0; k < N; k++) {
        setValor(C, i, j, ORDENXFILAS,
                 getValor(C, i, j, ORDENXFILAS) +
                     getValor(A, i, k, ORDENXFILAS) *
                         getValor(B, k, j, ORDENXFILAS));
      }
    }
  }
```

Execution time without fuctions overhead:
    5.116s
```c
 for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      C[i * N + j] = 0;
      for (k = 0; k < N; k++) {
        C[i * N + j] += A[i * N + k] * B[k * N + j];
      }
    }
  }
```

Both functions are not strictly necessary. As we can see, the overhead introduced by their calls makes the performance worse than it should be. Removing them improves the execution time by nearly 270%.

But why does this happen? It's a good question. As I mentioned, using functions in this case introduces a high overhead for what we really need, which is just multiplying matrices. At first sight, someone might think that it improves readability, but not necessarily — and as we discussed, it makes our code less efficient.

When we call a function in C, the program needs to perform several operations that add extra execution time. First, the function's arguments are pushed onto the stack, which is a reserved memory area used to store temporary data. Then, the processor needs to store the current instruction pointer (which indicates the next instruction to execute) so that it can return to the correct location after the function finishes. Inside the function, local variables are also allocated on the stack. Finally, the function's result is returned, and the stack is cleaned up before resuming the original code execution. These operations, especially when done repeatedly in a nested loop like this example, significantly impact performance.


### ii

Execution time without fuctions overhead and B stored by columns:
    3.251s

```c
for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      C[i * N + j] = 0;
      for (k = 0; k < N; k++) {
        C[i * N + j] += A[i * N + k] * B[k + N * j];
      }
    }
  }
```

As we can see, when we perform matrix multiplication $A \cdot B$, storing $B$ by columns instead of rows improves performance significantly.

Why is that? This behavior is related to cache memory and the principle of locality. When the CPU needs data from memory that is not already in cache, it fetches an entire cache line — a contiguous block of memory — instead of just the required data. The size of a cache line depends on the system architecture, but it is typically 32, 64, or 128 bytes. This mechanism improves performance by anticipating that nearby memory locations will likely be accessed soon (spatial locality).

The performance improvement comes from how matrix multiplication accesses memory. If $B$ is stored by rows, the innermost loop iterates over columns, which results in non-contiguous memory accesses. Each time a new column is accessed, the CPU is likely to fetch a new cache line, increasing the number of cache misses.

On the other hand, storing $B$ by columns ensures that the innermost loop accesses consecutive memory locations, maximizing cache hits and reducing the number of memory accesses. This small change takes better advantage of the CPU cache, which explains the significant performance difference.

## B

For all cases $N = 1024$


In all cases, there are two implemented ways to solve the operation.


### expMatrices1


$$
AB + AC + AD
$$

The first method, uses just one loop, and it's average time its: 
    10.410 s.


The second method uses four separate loops: one to compute $AB$, another for $AC$, and so on. Its average execution time is 9.981 s.

At first, this result might seem counterintuitive — four loops instead of one should be less efficient. However, after further analysis, I realized that the overhead introduced by the additional loops is smaller than the time needed to access memory.

But why does the first version access memory more frequently than the second one? The explanation lies in the cache size. In the first version, since all operations are performed inside a single loop, the amount of data being accessed simultaneously is larger. This likely causes cache blocks that are currently in use to be constantly overwritten by other blocks that the CPU also needs, increasing the number of cache misses.

In the second version, each loop processes a smaller amount of data at a time, which fits better in the cache, improving cache locality and reducing memory accesses.

### expMatrices2

$$
AB + CD
$$

The first method uses a single loop to perform the entire computation, with an average execution time of 7.049 s.

The second method, on the other hand, uses three separate loops: one to compute $AB$, another to compute $CD$, and a final loop to sum the resulting matrices. Its average execution time is 6.721 s.

Similarly to the previous case, the method that uses more loops is faster than the one that combines everything in a single loop. The reason behind this improvement is the same.

### expMatrices3

$$
BA + CAD
$$

The first method uses two loops to perform the entire computation, with an average execution time of 9.750 s.

The second method, on the other hand, uses four separate loops. Its average execution time is 9.981 s.
 
## C

$N = 1024$

For performing the matrix multiplication $AA$, two methods were used to test which one achieves better performance.

The first method consists of a straightforward matrix multiplication, with an average execution time of 4.001 s.

In the second method, an auxiliary matrix was introduced, where matrix $A$ is stored by columns — effectively the transpose of $A$. This method achieved an average execution time of 3.319 s.

## D

For $N = 1024$

| Block size |  Time  |
| :--------: | :----: |
| 4          | 5.078  |
| 8          | 3.812  |
| 16         | 3.667  |
| 32         | 3.642  |
| 64         | 3.626  |
| 128        | 3.631  |
| 256        | 3.599  |
| 512        | 3.532  |
| 1024       | 3.487  |

For $N = 2048$

| Block size |  Time  |
| :--------: | :----: |
| 512        | 28.490 |
| 1024       | 28.601 |        
| 2048       | 27.986 |        

After running the experiments multiple times and comparing the results with the version that does not use matrix multiplication by blocks, the non-blocked version consistently shows slightly better performance. This result is surprising because blocking is supposed to improve cache locality by processing smaller portions of the matrix at a time.

One possible explanation is that the implementation of the blocked version might introduce additional overhead from managing loops and temporary buffers, which could outweigh the benefits of cache optimization, especially for smaller matrix sizes. Additionally, the cache size and the way the blocking is applied may not be fully optimized for the system architecture.

## E

For performing the operations:

$$
MU ~~ ML ~~ UM ~~ LM
$$

there are two possible ways to store triangular matrices:

- Full storage: Storing the entire $N \times N$ matrix, including the zero elements in the unused triangular part.
- Compressed storage: Storing only the relevant triangular part, which reduces the size to $N \cdot (N + 1) / 2$ elements.

Both methods yield the same result, but the execution times differ.

In the first method, where the matrix is stored with zeros, the four operations take 9.096 s on average.

In the second method, where only the necessary elements are stored, the smaller matrix size improves performance, with an average execution time of 8.446 s.

The improvement happens because the compressed storage reduces the amount of memory accesses and allows better cache utilization, resulting in fewer cache misses and faster operations.

