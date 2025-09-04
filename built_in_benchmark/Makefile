CC := gcc

source_files := built_in_benchmark.c
compilation_out := built_in_benchmark_run

.PHONY: all clean

all:
	$(CC) $(source_files) -o $(compilation_out) -lm

clean:
	rm -f $(compilation_out)
