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

## For VSCode Users (No Make Required)

If you don't have `make` installed or prefer using direct g++ commands, we've got you covered!

### Option 1: Shell Script (Recommended)

The easiest way to compile and run all tests:

**On Mac/Linux:**
```bash
# First time only: make the script executable
chmod +x run_tests.sh

# Then run it:
./run_tests.sh
```

**On Windows (PowerShell):**
```powershell
# First time only: allow script execution
Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser

# Then run it:
.\run_tests.ps1
```

**On Windows (Git Bash):**
```bash
bash run_tests.sh
```

This script will:
1. Compile all test files
2. Run correctness tests automatically
3. Ask if you want to run performance tests
4. Handle any compilation errors gracefully

### Option 2: Individual Commands

Copy-paste these commands directly into your terminal:

```bash
# Test your LinkedList implementation
g++ --std=c++17 test_correctness.cpp ArrayList.cpp LinkedList.cpp -o test_correctness.out && ./test_correctness.out

# Run performance comparisons (after correctness passes)
g++ --std=c++17 test_performance.cpp ArrayList.cpp LinkedList.cpp -o test_performance.out && ./test_performance.out

# Test tail optimization (after implementing it)
g++ --std=c++17 test_tail_optimization.cpp ArrayList.cpp LinkedList.cpp -o test_tail_optimization.out && ./test_tail_optimization.out
```

### Option 3: VSCode Tasks (Most Integrated)

For keyboard shortcut integration:

1. The `.vscode/tasks.json` file is already included
2. Press **Cmd+Shift+B** (Mac) or **Ctrl+Shift+B** (Windows/Linux) to run tests
3. Or press **Cmd+Shift+P** → "Tasks: Run Task" to choose specific tests

### Quick Reference

See [COMPILE_COMMANDS.md](COMPILE_COMMANDS.md) for a complete command reference.

### Clean Up

To remove compiled files and results:

```bash
rm -f *.out *.csv *.png
```

---

## Files Included for VSCode Support

- `run_tests.sh` - Shell script for easy compilation and testing
- `.vscode/tasks.json` - VSCode task configuration for keyboard shortcuts
- `COMPILE_COMMANDS.md` - Quick reference for all compilation commands

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