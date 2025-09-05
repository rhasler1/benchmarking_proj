#!/bin/bash

EXECUTABLE="./interprocess_benchmark_run"
OUTPUT="results.txt"

sizes=(4 16 64 256 1024 4096 16384 65536 262144 524288)
#arr=(4 16)

echo "Starting $EXECUTABLE, output directed to $OUTPUT"
{
#$EXECUTABLE 4 0
for val in "${sizes[@]}"; do
    $EXECUTABLE "$val" 0
done
for val in "${sizes[@]}"; do
    $EXECUTABLE "$val" 1
done
} >> $OUTPUT 2>&1
echo "Done"
