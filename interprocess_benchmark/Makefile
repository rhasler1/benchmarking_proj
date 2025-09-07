CC := gcc

source_files := interprocess_benchmark.c
compilation_out := interprocess_benchmark_run
common_files := ../common/formula.c

.PHONY: all clean

all:
	$(CC) -I../common $(source_files) $(common_files) -o $(compilation_out) -lm

clean:
	rm -f $(compilation_out)
