# Exercise 3



 For $N = 100000000$, two programs were implemented to calculate the vectorial product between two vectors, where $N$ represents the size of the vectors.

The first program stores the vectors using a struct:

```c
typedef struct rvector {
  double i;
  double j;
  double k;
} Tvector;                
```

In this case, when memory is allocated, all three components of the vector (i, j, and k) are stored in a contiguous block of memory, which improves cache locality.

The second program stores each coordinate in separate arrays (without using structs). As a result, there is no guarantee that the allocated memory will be contiguous in RAM, potentially leading to worse cache performance.

The measured execution times were:

| With registers | Without registers | 
| :------------: | :---------------: |
|     879 ms     |       1.12 s      |

