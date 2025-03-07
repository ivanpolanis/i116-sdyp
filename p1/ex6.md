# Exercise 6

Apparently, the solution with the least overhead is the second one — the one that simply skips the selected position in the loop.


```c
   for (i = 0; i < posicionSeleccionada; i++) {
    if (v[i] < v[posicionSeleccionada])
      menores2++;
  }

  for (i = posicionSeleccionada + 1; i < N; i++) {
    if (v[i] < v[posicionSeleccionada])
      menores2++;
  }
```
