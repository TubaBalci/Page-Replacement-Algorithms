# Page-Replacement-Algorithms
Popular page replacement algoritms like FIFO, LRU, LFU...

This C program simulates memory page replacement algorithms, specifically focusing on five different strategies: FIFO (First-in first-out), LRU (Least Recently Used), LFU (Least Frequently Used), Random, and Weighted Random. The program takes input from the "input.txt" file, where each line represents an access list for a hypothetical process. The goal is to analyze and compare the performance of these algorithms based on the number of page faults.

This program explores different page replacement algorithms used in operating systems to manage memory efficiently. The algorithms are tested on various access lists to evaluate their performance in terms of page faults. The memory size is specified as a command-line argument when running the program.

# Page Replacement Algorithms
FIFO (First-in first-out)
FIFO is a page replacement algorithm that replaces the oldest page in memory. It follows a first-in, first-out order.

LRU (Least Recently Used)
LRU replaces the page that has not been used for the longest time. It keeps track of the access time of each page.

LFU (Least Frequently Used)
LFU replaces the page that has been accessed the least frequently. It tracks both the access count and the access time.

Random
Random selects a page in memory randomly for replacement, with equal probabilities for all pages.

Weighted Random
Weighted Random selects a page based on a weighted random approach, where the probability of selecting a page is inversely proportional to its access count.

# Results
The program reads access lists from "input.txt" and executes each of the page replacement algorithms. The number of page faults for each algorithm is printed to the console. For Random and Weighted Random algorithms, the program runs 1000 iterations to obtain a more stable average result.
