EXECUTABLE="./simple_benchmark_run"
ITERATIONS=0

# iterations from 10^2..10^10
for ((i=2;i<11;i++)); do
	ITERATIONS=$((10**i));
	echo "Run with $ITERATIONS iterations"
	time $EXECUTABLE $ITERATIONS
done
echo "Done"
