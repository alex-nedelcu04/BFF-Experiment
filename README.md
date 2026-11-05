# BFF Self-Replication Experiment - C++

## Nedelcu Alexandru-Constantin (341) & Săndulescu Ioan (341)

This project searches for self-replicating 64-byte programs using a modified Brainfuck interpreter (BFF), based on the paper [BFF: Simple explanations for complex phenomena](https://arxiv.org/abs/2607.01483).

It tests three byte distributions (UNIFORM, CUST, CUST64) and four mutation probabilities (1/200, 1/100, 1/50, 1), evaluating each candidate through nine parallel trials of five generations.

## How to Use

Compile and run:

```bash
g++ -O3 -std=c++20 -pthread main.cpp -o bff
./bff
```

No input files are required. Each configuration tests 5,000,000 programs.

## Experimental Results

Position of the first detected self-replicator among 5,000,000 tested programs per configuration:

| Distribution | p | Run 1 | Run 2 | Run 3 | Run 4 | Run 5 | Average | Median |
|---|---|---:|---:|---:|---:|---:|---:|---:|
| UNIFORM | 1/200 | - | 2,119,999 | - | - | - | 2,119,999 | 2,119,999 |
| UNIFORM | 1/100 | - | - | - | - | - | N/A | N/A |
| UNIFORM | 1/50 | - | - | 3,534,419 | - | - | 3,534,419 | 3,534,419 |
| UNIFORM | 1 | 3,814,682 | 233,789 | 2,247,148 | 403,930 | 2,833,232 | 1,906,556 | 2,247,148 |
| CUST | 1/200 | 4,704,036 | - | 3,505,479 | - | 493,552 | 2,901,022 | 3,505,479 |
| CUST | 1/100 | 3,618,522 | 2,158,811 | 904,256 | 3,418,785 | 3,944,610 | 2,808,997 | 3,418,785 |
| CUST | 1/50 | 797,229 | 1,377,449 | 345,854 | 1,104,986 | - | 906,380 | 951,108 |
| CUST | 1 | 542,297 | 1,287,573 | 284,243 | 617,352 | 789,581 | 704,209 | 617,352 |
| CUST64 | 1/200 | 181,177 | 3,010,402 | 684,667 | 4,156,945 | 2,390,858 | 2,084,810 | 2,390,858 |
| CUST64 | 1/100 | 1,081,220 | 1,042,266 | 2,383,460 | 364,041 | 580,020 | 1,090,201 | 1,042,266 |
| CUST64 | 1/50 | 4,051,995 | 740,943 | 46,127 | 168,215 | 932,217 | 1,187,899 | 740,943 |
| **CUST64** | **1** | **28,673** | **161,302** | **92,223** | **237,971** | **282,934** | **160,621** | **161,302** |

`-` indicates no replicator found within 5,000,000 programs. Averages and medians exclude unsuccessful runs.

Full outputs:
- [output_run1.out](./output_run1.out) (longer runtime due to a weaker CPU)
- [output_run2.out](./output_run2.out)
- [output_run3.out](./output_run3.out)
- [output_run4.out](./output_run4.out)
- [output_run5.out](./output_run5.out)

## Conclusion

Across five runs, **CUST64 with `p = 1` consistently performed best**, finding replicators after an average of 160,621 programs. Higher mutation probabilities generally improved discovery rates, while lower probabilities produced more variable results. These findings support the effectiveness of CUST64, although additional runs would improve statistical reliability.
