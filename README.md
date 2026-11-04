# BFF Self-Replication Experiment - C++

## Nedelcu Alexandru-Constantin (341) & Săndulescu Ioan (341)

The program reproduces a random-mutation search for self-replicating programs using a modified Brainfuck interpreter (BFF). It generates 64-byte programs using three distributions (UNIFORM, CUST, CUST64) and four mutation probabilities (1/200, 1/100, 1/50, 1). Each candidate is evaluated through nine parallel trials of five generations, with a self-replication score threshold of 48/64. The program records unique self-replicators, the number of candidates tested before the first detection, and total execution time.

## How to Use

1. Compile the program with C++20 and threading support:

   ```bash
   g++ -O3 -std=c++20 -pthread main.cpp -o bff
   ```

2. Run the experiment:

   ```bash
   ./bff
   ```

3. Results are saved to `output.out`, including the detected programs in hexadecimal format.

**Note:** No input files are required. Each configuration tests up to 5,000,000 candidate programs, with a population size of 1,000. `output.out` is created automatically.

## Experimental Results

| Distribution | Mutation probability | First replicator (programs tested) | Unique replicators | Execution time (s) |
|---|---:|---:|---:|---:|
| UNIFORM | 1/200 | Not found | 0 | 1692.401 |
| UNIFORM | 1/100 | Not found | 0 | 1687.088 |
| UNIFORM | 1/50 | Not found | 0 | 1684.490 |
| UNIFORM | 1 | 3,814,682 | 2 | 1738.027 |
| CUST | 1/200 | 4,704,036 | 1 | 1737.395 |
| CUST | 1/100 | 3,618,522 | 4 | 1740.831 |
| CUST | 1/50 | 797,229 | 1 | 1738.782 |
| CUST | 1 | 542,297 | 14 | 1746.367 |
| CUST64 | 1/200 | 181,177 | 36 | 1733.199 |
| CUST64 | 1/100 | 1,081,220 | 21 | 1731.173 |
| CUST64 | 1/50 | 4,051,995 | 4 | 1732.213 |
| CUST64 | 1 | **28,673** | **45** | 1741.267 |

**Conclusion:** In this run, CUST64 with mutation probability `p = 1` found the first self-replicator fastest and produced the most unique detected programs.

The complete experimental results, including all detected self-replicators and their hexadecimal representations, are available in [output.txt](./output.txt).