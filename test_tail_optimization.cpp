#include "ArrayList.h"
#include "LinkedList.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>

using namespace std;

const vector<int> SIZES = {100, 500, 1000, 2000, 5000};

template<typename T>
int benchmark_append(T& ds, int size) {
    int total_ops = 0;
    for (int i = 0; i < size; i++) {
        total_ops += ds.append(i);
    }
    return total_ops;
}

int main() {
    cout << "============================================================" << endl;
    cout << "TAIL POINTER OPTIMIZATION TEST" << endl;
    cout << "============================================================" << endl;
    cout << "\nTesting your optimized LinkedList append performance..." << endl;
    cout << "If you've correctly added the tail pointer, append should" << endl;
    cout << "now be O(1) instead of O(n)!\n" << endl;
    
    vector<tuple<int, int, int>> results;  // size, array_ops, linked_ops
    
    for (int size : SIZES) {
        cout << "Testing size " << size << "..." << endl;
        
        ArrayList arr;
        int array_ops = benchmark_append(arr, size);
        cout << "  ArrayList:  " << setw(10) << array_ops << " operations" << endl;
        
        LinkedList ll;
        int linked_ops = benchmark_append(ll, size);
        cout << "  LinkedList: " << setw(10) << linked_ops << " operations" << endl;
        
        results.push_back({size, array_ops, linked_ops});
    }
    
    // Check if optimization worked
    cout << "\n============================================================" << endl;
    cout << "ANALYSIS:" << endl;
    cout << "============================================================" << endl;
    
    auto [largest_size, largest_array, largest_linked] = results.back();
    if (largest_linked <= largest_size * 2) {  // Allow some overhead
        cout << "✅ SUCCESS! Your LinkedList append is now O(1)!" << endl;
        cout << "   Each append takes constant time, so total is O(n)." << endl;
        cout << "   For " << largest_size << " appends: " << largest_linked << " operations" << endl;
        cout << "   That's approximately 1 operation per append!" << endl;
    } else {
        cout << "❌ Hmm, LinkedList still seems slow." << endl;
        cout << "   Double-check that you:" << endl;
        cout << "   1. Added Node* tail to LinkedList.h" << endl;
        cout << "   2. Initialized tail = nullptr in constructor" << endl;
        cout << "   3. Updated append() to use tail" << endl;
        cout << "   4. Maintained tail in prepend() and remove()" << endl;
    }
    
    // Save results for plotting
    ofstream file("tail_optimization_results.csv");
    file << "size,array_ops,linked_ops,old_linked_ops\n";
    for (const auto& [size, array_ops, linked_ops] : results) {
        int old_ops = (size * (size + 1)) / 2;  // O(n²) estimate
        file << size << "," << array_ops << "," << linked_ops << "," << old_ops << "\n";
    }
    file.close();
    
    cout << "\n💾 Results saved to: tail_optimization_results.csv" << endl;
    cout << "\n============================================================" << endl;
    cout << "COMPARISON:" << endl;
    cout << "============================================================" << endl;
    cout << "LinkedList BEFORE tail pointer: O(n²) total operations" << endl;
    cout << "LinkedList AFTER tail pointer:  O(n) total operations" << endl;
    cout << "Now it matches ArrayList's performance!" << endl;
    cout << "============================================================" << endl;
    
    return 0;
}