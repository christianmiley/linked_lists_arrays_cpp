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

# Compile correctness tests
g++ --std=c++17 test_correctness.cpp ArrayList.cpp LinkedList.cpp -o test_correctness.out
if [ $? -ne 0 ]; then
    echo "❌ Compilation failed for correctness tests"
    echo "   Make sure all .cpp and .h files are in this directory"
    exit 1
fi

# Compile performance tests
g++ --std=c++17 test_performance.cpp ArrayList.cpp LinkedList.cpp -o test_performance.out
if [ $? -ne 0 ]; then
    echo "❌ Compilation failed for performance tests"
    exit 1
fi

# Compile tail optimization test
g++ --std=c++17 test_tail_optimization.cpp ArrayList.cpp LinkedList.cpp -o test_tail_optimization.out
if [ $? -ne 0 ]; then
    echo "❌ Compilation failed for tail optimization test"
    exit 1
fi

echo "✅ All files compiled successfully!"
echo ""

# Run correctness tests
echo "Running correctness tests..."
./test_correctness.out

if [ $? -ne 0 ]; then
    echo ""
    echo "⚠️  Correctness tests failed. Fix your LinkedList implementation first."
    exit 1
fi

# Ask before running performance tests
echo ""
read -p "✅ Correctness tests passed! Run performance tests? (y/n): " response
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
echo "🎉 Done!"