CC := gcc

source_files := nontrivial_ops.c
compilation_out := nontrivial_ops_run
common_files := ../common/formula.c

.PHONY: all clean

all:
	$(CC) -I../common $(source_files) $(common_files) -o $(compilation_out) -lm

clean:
	rm -f $(compilation_out)
