CC := gcc

source_files := interprocess_benchmark.c
compilation_out := interprocess_benchmark_run

.PHONY: all clean

all:
	$(CC) $(source_files) -o $(compilation_out)

clean:
	rm -f $(compilation_out)