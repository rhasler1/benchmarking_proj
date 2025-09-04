#!/bin/bash

EXECUTABLE="./built_in_benchmark_run"
OUTPUT="results.txt"
LOOP_LENGTH=0

echo "Starting $EXECUTABLE, output directed to $OUTPUT"
{
# loop length from 10^2..10^10
for ((i=2;i<11;i++)); do
	LOOP_LENGTH=$((10**i));
	$EXECUTABLE $LOOP_LENGTH
done
} >> $OUTPUT 2>&1

echo "Done"
