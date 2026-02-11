# Data Structures Performance Comparison (C++)

Compare ArrayList (vector wrapper) vs LinkedList performance across different operations.

## Building
```bash
make
```

The Makefile automates compilation. Here's what it does:
- `make` or `make all` - Compiles all executables
- `make clean` - Removes compiled files
- Each `.cpp` file is compiled to a `.o` object file first
- Object files are linked together to create executables

Running `make` creates:
- `test_correctness` - Unit tests
- `test_performance` - Performance benchmarks
- `test_tail_optimization` - Tests the tail pointer optimization
- `run_all_tests` - Runs correctness then performance tests

## Running

### Run everything:
```bash
./run_all_tests
```

### Or run individually:
```bash
./test_correctness
./test_performance
./test_tail_optimization
```

## Assignment Flow

1. Implement methods in `LinkedList.cpp` (ArrayList is provided)
2. Run `./run_all_tests` until all tests pass and you go through 5 challenges
3. Follow extension instructions to add tail pointer
4. Run `./test_tail_optimization` to verify optimization

## Visualizing Results (Optional)

Performance tests save results to CSV files (`challenge_1_results.csv`, etc.).

If you have Python 3 with pandas and matplotlib:
```bash
python3 plot_results.py
```
This generates PNG plots for all challenges.

## Clean Up
```bash
make clean
```