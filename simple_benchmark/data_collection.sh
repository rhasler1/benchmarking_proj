#!/bin/bash

EXECUTABLE="./simple_benchmark_run"
OUTPUT="results.txt"
ITERATIONS=0
AVG_TIME=0

echo "Starting simple_benchmark script, output directed to $OUTPUT"
{
# iterations from 10^2..10^10
for ((i=2;i<11;i++)); do
	ITERATIONS=$((10**i));
	echo "Beginning data collection for $ITERATIONS iterations"
	for ((j=0;j<20;j++)); do
		echo "Run number: $j"
		time $EXECUTABLE $ITERATIONS
		echo
	done
echo "Iterations $ITERATIONS completed"
done
} >> $OUTPUT 2>&1

echo "Done"
