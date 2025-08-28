#!/bin/bash

EXECUTABLE="./nontrivial_ops_run"
OUTPUT="results.txt"
ITERATIONS=0

echo "Starting $EXECUTABLE, output directed to $OUTPUT"

# benchmark getpid()
{
echo "Benchmarking getpid()"
for ((i=0;i<20;i++)); do
	echo "Run number: $i"
	$EXECUTABLE 0
	echo
done
} >> $OUTPUT 2>&1

echo "Done"
