#!/bin/bash

################################################################################
# run_tests.sh
# 
# Shell script for compiling and running all tests without make.
# Perfect for VSCode users who want a simple compilation workflow.
# 
# Usage:
#   bash run_tests.sh
# 
# This script will:
#   1. Compile all test files
#   2. Run correctness tests
#   3. Ask if you want to run performance tests
#   4. Optionally run performance tests
################################################################################

echo "============================================================"
echo "  DATA STRUCTURES PERFORMANCE COMPARISON"
echo "  ArrayList vs LinkedList"
echo "============================================================"
echo ""
echo "Compiling tests..."

# Compile LinkedList tests
g++ --std=c++17 test_linkedlist.cpp ArrayList.cpp LinkedList.cpp -o test_linkedlist.out 2>&1
if [ $? -ne 0 ]; then
    echo "[X] Compilation failed for LinkedList tests"
    echo "    Make sure all .cpp and .h files are in this directory"
    exit 1
fi

# Compile performance tests
g++ --std=c++17 test_performance.cpp ArrayList.cpp LinkedList.cpp -o test_performance.out 2>&1
if [ $? -ne 0 ]; then
    echo "[X] Compilation failed for performance tests"
    exit 1
fi

# Compile tail optimization test
g++ --std=c++17 test_tail_optimization.cpp ArrayList.cpp LinkedList.cpp -o test_tail_optimization.out 2>&1
if [ $? -ne 0 ]; then
    echo "[X] Compilation failed for tail optimization test"
    exit 1
fi

echo "[OK] All files compiled successfully!"
echo ""

# Run LinkedList tests and capture output
echo "Running LinkedList tests..."
test_output=$(./test_linkedlist.out)
echo "$test_output"

# Check if all tests passed by looking for the success message
if echo "$test_output" | grep -q "Total: [0-9]*/[0-9]* tests passed" && \
   echo "$test_output" | grep -q "\[SUCCESS\] All tests passed"; then
    echo ""
    echo "[OK] All LinkedList tests passed!"
else
    echo ""
    echo "[!] LinkedList tests failed. Fix your LinkedList implementation first."
    echo "    Review the errors above to see which functions need fixing."
    exit 1
fi

# Ask before running performance tests
echo ""
read -p "[OK] Ready for performance tests. Continue? (y/n): " response
if [[ "$response" == "y" || "$response" == "Y" ]]; then
    ./test_performance.out
else
    echo "Performance tests skipped."
    echo ""
    echo "To run performance tests later, use:"
    echo "  ./test_performance.out"
    echo ""
    echo "To test tail optimization (after implementing it), use:"
    echo "  ./test_tail_optimization.out"
fi

echo ""
echo "Done!"