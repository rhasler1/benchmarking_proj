CC := gcc

source_files := main.c
compilation_out := main

.PHONY: all clean

all:
	$(CC) main.c formula.c -o $(compilation_out) -lm

clean:
	rm -f $(compilation_out)
