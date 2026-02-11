CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

# Object files
OBJS = ArrayList.o LinkedList.o

# Executables
TARGETS = test_correctness test_performance test_tail_optimization run_all_tests

all: $(TARGETS)

# Compile object files
ArrayList.o: ArrayList.cpp ArrayList.h
	$(CXX) $(CXXFLAGS) -c ArrayList.cpp

LinkedList.o: LinkedList.cpp LinkedList.h
	$(CXX) $(CXXFLAGS) -c LinkedList.cpp

# Build executables
test_correctness: test_correctness.cpp $(OBJS)
	$(CXX) $(CXXFLAGS) -o test_correctness test_correctness.cpp $(OBJS)

test_performance: test_performance.cpp $(OBJS)
	$(CXX) $(CXXFLAGS) -o test_performance test_performance.cpp $(OBJS)

test_tail_optimization: test_tail_optimization.cpp $(OBJS)
	$(CXX) $(CXXFLAGS) -o test_tail_optimization test_tail_optimization.cpp $(OBJS)

run_all_tests: run_all_tests.cpp
	$(CXX) $(CXXFLAGS) -o run_all_tests run_all_tests.cpp

clean:
	rm -f $(OBJS) $(TARGETS) *.csv

.PHONY: all clean