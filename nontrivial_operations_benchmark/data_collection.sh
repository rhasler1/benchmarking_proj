#!/bin/bash

EXECUTABLE="./nontrivial_ops_run"
OUTPUT="results.txt"

echo "Starting $EXECUTABLE, output directed to $OUTPUT"
{
$EXECUTABLE 0
$EXECUTABLE 1
$EXECUTABLE 2
} >> $OUTPUT 2>&1
echo "Done"
