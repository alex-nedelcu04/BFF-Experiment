# BFF Self-Replication Experiment - C++

## Nedelcu Alexandru-Constantin (341) & Săndulescu Ioan (341)

This project searches for self-replicating 64-byte programs using a modified Brainfuck interpreter (BFF), based on the paper [BFF: Simple explanations for complex phenomena](https://arxiv.org/abs/2607.01483).

It tests three byte distributions (UNIFORM, CUST, CUST64) and four mutation probabilities (1/200, 1/100, 1/50, 1), evaluating each candidate through nine parallel trials of five generations.

## How to Use

Compile and run:

```bash
g++ -O3 -std=c++20 -pthread bff_experiment.cpp -o bff
./bff
```

No input files are required. Each configuration tests 5,000,000 programs.

## Experimental Results

Each cell shows **first self-replicator position / number of unique self-replicators**.

| Distribution | p | Run 1 | Run 2 | Run 3 | Run 4 | Run 5 | Average | Median |
|---|---|---:|---:|---:|---:|---:|---:|---:|
| UNIFORM | 1/200 | - / 0 | 2,119,999 / 11 | - / 0 | - / 0 | - / 0 | 2,119,999 / 2.2 | 2,119,999 / 0 |
| UNIFORM | 1/100 | - / 0 | - / 0 | - / 0 | - / 0 | - / 0 | N/A / 0 | N/A / 0 |
| UNIFORM | 1/50 | - / 0 | - / 0 | 3,534,419 / 2 | - / 0 | - / 0 | 3,534,419 / 0.4 | 3,534,419 / 0 |
| UNIFORM | 1 | 3,814,682 / 2 | 233,789 / 1 | 2,247,148 / 1 | 403,930 / 2 | 2,833,232 / 1 | 1,906,556 / 1.4 | 2,247,148 / 1 |
| CUST | 1/200 | 4,704,036 / 1 | - / 0 | 3,505,479 / 7 | - / 0 | 493,552 / 3 | 2,901,022 / 2.2 | 3,505,479 / 1 |
| CUST | 1/100 | 3,618,522 / 4 | 2,158,811 / 24 | 904,256 / 2 | 3,418,785 / 9 | 3,944,610 / 1 | 2,808,997 / 8 | 3,418,785 / 4 |
| CUST | 1/50 | 797,229 / 1 | 1,377,449 / 3 | 345,854 / 2 | 1,104,986 / 4 | - / 0 | 906,380 / 2 | 951,108 / 2 |
| CUST | 1 | 542,297 / 14 | 1,287,573 / 7 | 284,243 / 10 | 617,352 / 8 | 789,581 / 12 | 704,209 / 10.2 | 617,352 / 10 |
| CUST64 | 1/200 | 181,177 / 36 | 3,010,402 / 7 | 684,667 / 33 | 4,156,945 / 8 | 2,390,858 / 18 | 2,084,810 / 20.4 | 2,390,858 / 18 |
| CUST64 | 1/100 | 1,081,220 / 21 | 1,042,266 / 7 | 2,383,460 / 16 | 364,041 / 8 | 580,020 / 22 | 1,090,201 / 14.8 | 1,042,266 / 16 |
| CUST64 | 1/50 | 4,051,995 / 4 | 740,943 / 17 | 46,127 / 12 | 168,215 / 13 | 932,217 / 12 | 1,187,899 / 11.6 | 740,943 / 12 |
| **CUST64** | **1** | **28,673 / 45** | **161,302 / 29** | **92,223 / 29** | **237,971 / 30** | **282,934 / 42** | **160,621 / 35** | **161,302 / 30** |

`-` indicates no self-replicator detected within 5,000,000 programs. First-detection averages and medians exclude unsuccessful runs; unique-replicator statistics include all five runs. Unique counts are calculated separately for each configuration.

Full outputs:
- [output_run1.out](./output_run1.out) (longer runtime due to a weaker CPU)
- [output_run2.out](./output_run2.out)
- [output_run3.out](./output_run3.out)
- [output_run4.out](./output_run4.out)
- [output_run5.out](./output_run5.out)

## Conclusion

Across five runs, **CUST64 with `p = 1` performed best**, detecting its first self-replicator after an average of 160,621 programs and finding an average of 35 unique self-replicators per run. CUST64 produced the most unique self-replicators overall (409 out of 541 configuration-level counts). Higher mutation probabilities generally improved discovery speed, while lower probabilities showed greater variability.
