# Code Repository for Benchmarking Project Paper

## /Common
Contains mean and standard deviation formulas

## /simple_benchmark, /built_in_benchmark, /nontrivial, & /interprocess
Contain respective source code, Makefile, and data collection bash script

## /data_file
Contains output from data_collection scripts when run on target hardware.
This is the data used in the paper.

### Target Device CPU Info
Architecture:             aarch64
  CPU op-mode(s):         32-bit, 64-bit
  Byte Order:             Little Endian
CPU(s):                   4
  On-line CPU(s) list:    0-3
Vendor ID:                ARM
  Model name:             Cortex-A76
    Model:                1
    Thread(s) per core:   1
    Core(s) per cluster:  4
    Socket(s):            -
    Cluster(s):           1
    Stepping:             r4p1
    CPU(s) scaling MHz:   67%
    CPU max MHz:          2400.0000
    CPU min MHz:          1500.0000

RAM:                      8 GB
SD Card:                  128 GB

#### instructions: https://neilklingensmith.com/teaching/loyola/cs410-f2025/homework/benchmarking.pdf

