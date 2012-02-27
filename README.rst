========
Chromatic
========

This is a randomized genetic algorithm for computing an upper bound on the chromatic 
number of a graph.


Running
============
* Compilation
    $ g++ chromatic.cpp -O3 -o chrom

* Preprocess input graph
Use the mathematica notebook preprocess.nb to generate the optimized data structure in binary format. Change $graphPath and $repoPath, and run notebook.

* Running
    $ ./chrom

Benchmarks
========
Graph	        best K/CHI
dsjc1000.1	20/?	
dsjc1000.5	85/?	
dsjc1000.9	223/?	
dsjr500.1c	84/84	
dsjr500.5	122/122
