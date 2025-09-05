CC := gcc

source_files := nontrivial_ops.c
compilation_out := nontrivial_ops_run

.PHONY: all clean

all:
	$(CC) $(source_files) -o $(compilation_out) -lm

clean:
	rm -f $(compilation_out)
