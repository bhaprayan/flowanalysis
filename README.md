# Microsoft Research India Assignment

Implementation of algorithms for packet flow analysis (limited to TCP and UDP flows).
Contains two variants of a program to compute the packet count:
1. Exact count without dealing with space complexity
2. Optimized implementation returning an approximate count (within a specific bound) using a probablistic algorithm/data structure (count-min sketch)


##Directory Layout

.
├── data ... Contains packet dumps for offline analysis
│   
├── helper ... Helper functions used during development
│   
├── plots ... Plots of frequency distributions of packet flows
│   
├── TaskOne ... Variant One (using a simple hash table to compute exact count)
│   ├── a.out
│   ├── HIST_FILE.txt ... Stores frequencies of unique flow tuples 
│   ├── packet_analysis.cc
│   ├── packet_analysis.h
│   ├── plot_hist.py ... Generates frequency distribution plots from HIST_FILE
│   └── profiling ... Valgrind massif memory profiling
└── TaskTwo ... Variant Two (optimized using a count-min sketch, returns approximate count within bounds)
    ├── a.out
    ├── count_min_sketch.cc
    ├── count_min_sketch.h
    ├── FLOW_FILE.txt ... Stores the unique flow tuples (generated from TaskOne)
    ├── packet_analysis.cc
    ├── packet_analysis.h
    └── profiling

##Basic Usage
Note: Significance of generated files is specified in the directory layout section
1. Compile and run the TaskOne/packet_analysis.cc
```Bash
g++ packet_analysis.cc -lpcap
./a.out [NAME OF DATA FILE]
```
2. This will generate HIST_FILE.txt and FLOW_FILE.txt (in the TaskTwo directory)
3. Compile and run the TaskTwo/packet_analysis.cc
```Bash
g++ packet_analysis.cc count_min_sketch.cc -lpcap
./a.out [NAME OF DATA FILE]
```
4. This will generate the count-min sketch (by analyzing packets from the input data file), and then estimate the frequency by reading in unique tuples from FLOW_FILE (generated in TaskOne to avoid having to manually enter flow tuples during the estimation phase).

##Advanced Usage (Profiling)
1. To profile the memory usage (heap) of the two variants using Valgrind
```Bash
valgrind --tool=massif ./a.out [NAME OF DATA FILE]
```
2. This will generate a massif.out.pid file, which can be analyzed using ms_print
```Bash
ms_print massif.out.pid file
```
3. To only display the graph of number of instructions VS memory usage
```Bash
ms_print massif.out.pid | head -n 35
```

##Future Work
1. Generate a frequency distribution plot for flow counts using HIST_FILE
```Bash
python hist_plot.py
```
2. Depending on the degree of skewness, choose which algorithm to run (either CMS or X). CMS has been observed to have better performance in skewed distributions, whereas X is better for non-skewed distributions.

##References
[1] http://dimacs.rutgers.edu/~graham/pubs/papers/cm-full.pdf
[2] https://en.wikipedia.org/wiki/Count%E2%80%93min_sketch#cite_note-goyal-5
[3] http://www.aclweb.org/anthology/D12-1100
[4] http://web.stanford.edu/class/cs168/l/l2.pdf


Maybe use CMS for central router grids

The other algorithm for a client server architecture over the internet (Google, Bing, etc)
