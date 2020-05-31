# analysis report

## My operationg system is MacOS so I use "g++ -Xpreprocessor -fopenmp -lomp ConwaysGame_Parallel.cpp " to compile cpp file.
## Use "./a.out" to run the code.

The performance is as following:

serial:                         0.751044s
parallel (2 threads):           0.563629s
parallel (4 threads):           0.343318s
parallel (8 threads):           0.241659s
parallel (16 threads):          0.237185s
parallel (32 threads):          0.243735s

When threads is more than 8, the performance can hardly improve.
Because my computer only have 8 threads. 
If I call for more than 8 threads, they can not work at same time.
Thus choosing suitable number of threads is important for good performance.
