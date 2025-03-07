# Exercise 5

## a

|   N    |    R    | not using `register` | using `register` |  
| :----: | :-----: | :------------------: | :--------------: |
| $2^{20}$ | $10^4$  |       8.504 s        |      3.602       |

As we can see, the version that uses the register keyword significantly outperforms the one that does not.

The key difference is that in the second version, the address of the array is stored in a register variable at the beginning of each loop iteration:

```c
for(r=0;r<R;r++){
	 for(i=0;i<N;i++){
		a[i] = 1;
 	}
 }
```

```c
 register unsigned long *p;
 for(r=0;r<R;r++){
 	for(p=a ; p < a+N ; p++){
		*p = 1;
 	}
 }   
```

By using a pointer variable declared as register, the processor stores the pointer's address in a CPU register instead of accessing memory repeatedly. This eliminates the need to calculate the address of a[i] on each iteration, improving performance by reducing memory accesses.

## b

The error when $N = 2147483648$ is due to the range of the data type representation. The following table summarizes the types of interest:

| Type | Storage size | Value range |
| :--: | :----------: | :---------: |
| int  |    4 bytes   | -2,147,483,648 to 2,147,483,647 |
| unsigned int  |    4 bytes   | 0 to 4,294,967,295 |
| unsigned long |    8 bytes   | 0 to 18446744073709551615 |


When the loop counter reaches 2147483648, it exceeds this limit, causing an overflow. In C, signed integers typically use two's complement representation, so the value wraps around to the minimum representable integer, which is -2,147,483,648. This behavior is defined by the C standard for signed integer overflow.
