# Exercise 8


The N-Queens problem consists of placing N queens on an N × N chessboard in such a way that no two queens threaten each other — meaning no two queens share the same row, column, or diagonal.

After some research, I found that this problem has a time complexity of O(N!). The values obtained for board sizes from 4 to 19 are:

|  N   | Solutions found |  Time  |
| :--: | :-------------: | :----: |
|  4   |         2       | 2.14 us |
|  5   |        10       | 953  ns |
|  6   |         4       | 953  ns |
|  7   |        40       | 2.86 us |
|  8   |        92       | 7.86 us |
|  9   |       352       | 28.8 us |
|  10  |       724       | 110  us |
|  11  |       2680      | 441  us |
|  12  |      14200      | 2.01 ms |
|  13  |      73712      | 10.9 ms |
|  14  |     365596      | 60.9 ms |
|  15  |    2279184      | 376  ms |
|  16  |   14772512      | 2.45  s |
|  17  |   95815104      | 17.1  s |
|  18  |  666090624      | 123   s |
|  19  | 4968057848      | 953   s |

These results show how the execution time increases exponentially as N grows, consistent with the factorial time complexity.
