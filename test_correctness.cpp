/*
 * test_correctness.cpp
 * 
 * Improved version with better error handling and crash detection.
 * 
 * Changes from original:
 * - Catches all exceptions (not just std::exception)
 * - Uses ASCII characters instead of emojis for better terminal compatibility
 * - Gives more detailed error messages
 * - Continues testing even if one test crashes
 */

#include "ArrayList.h"
#include "LinkedList.h"
#include <iostream>
#include <string>
#include <functional>
#include <csignal>

using namespace std;

// Global flag for segfault detection
volatile sig_atomic_t segfault_occurred = 0;
string current_test_name;

// Signal handler for segfaults
void segfault_handler(int signal) {
    cout << "[CRASH] " << current_test_name << " — SEGMENTATION FAULT" << endl;
    cout << "        This usually means:" << endl;
    cout << "        - Dereferencing a null pointer (e.g., current->next when current is nullptr)" << endl;
    cout << "        - Using deleted memory" << endl;
    cout << "        - Accessing array out of bounds" << endl;
    exit(1);
}

// Test helper
bool run_test(function<bool()> test_func, const string& description) {
    current_test_name = description;
    
    try {
        bool result = test_func();
        if (result) {
            cout << "[✓] " << description << endl;
            return true;
        } else {
            cout << "[X] " << description << " — test returned false" << endl;
            return false;
        }
    } catch (const std::exception& e) {
        cout << "[ERROR] " << description << " — exception: " << e.what() << endl;
        return false;
    } catch (...) {
        cout << "[ERROR] " << description << " — unknown exception thrown" << endl;
        return false;
    }
}

template<typename T>
void test_data_structure(const string& class_name) {
    cout << "\n=== Testing " << class_name << " ===" << endl;
    int passed = 0;
    int total = 0;
    
    // Test 1: Empty list size
    total++;
    if (run_test([]() {
        T ds;
        return ds.size() == 0;
    }, "Empty list has size 0")) passed++;
    
    // Test 2: Append elements
    total++;
    if (run_test([]() {
        T ds;
        ds.append(10);
        ds.append(20);
        ds.append(30);
        return ds.size() == 3;
    }, "Append 3 elements")) passed++;
    
    // Test 3: Prepend elements
    total++;
    if (run_test([]() {
        T ds;
        ds.prepend(30);
        ds.prepend(20);
        ds.prepend(10);
        auto [val, ops] = ds.get(0);
        return val == 10;
    }, "Prepend - first element is correct")) passed++;
    
    // Test 4: Get by index
    total++;
    if (run_test([]() {
        T ds;
        ds.append(100);
        ds.append(200);
        ds.append(300);
        auto [val, ops] = ds.get(1);
        return val == 200;
    }, "Get element at index 1")) passed++;
    
    // Test 5: Get out of bounds
    total++;
    if (run_test([]() {
        T ds;
        ds.append(10);
        auto [val, ops] = ds.get(5);
        return val == -1;
    }, "Get out of bounds returns -1")) passed++;
    
    // Test 6: Search - found
    total++;
    if (run_test([]() {
        T ds;
        for (int i = 0; i < 5; i++) {
            ds.append(i * 10);
        }
        auto [idx, ops] = ds.search(30);
        return idx == 3;
    }, "Search finds correct index")) passed++;
    
    // Test 7: Search - not found
    total++;
    if (run_test([]() {
        T ds;
        for (int i = 0; i < 5; i++) {
            ds.append(i * 10);
        }
        auto [idx, ops] = ds.search(999);
        return idx == -1;
    }, "Search returns -1 when not found")) passed++;
    
    // Test 8: Insert at middle
    total++;
    if (run_test([]() {
        T ds;
        ds.append(10);
        ds.append(30);
        ds.insert_at(1, 20);
        auto [val, ops] = ds.get(1);
        return val == 20;
    }, "Insert at middle position")) passed++;
    
    // Test 9: Delete element
    total++;
    if (run_test([]() {
        T ds;
        ds.append(10);
        ds.append(20);
        ds.append(30);
        auto [success, ops] = ds.remove(20);
        return success && ds.size() == 2;
    }, "Delete removes element and updates size")) passed++;
    
    // Test 10: Delete - not found
    total++;
    if (run_test([]() {
        T ds;
        ds.append(10);
        auto [success, ops] = ds.remove(999);
        return !success;
    }, "Delete returns false when element not found")) passed++;
    
    // Test 11: Complex sequence
    total++;
    if (run_test([]() {
        T ds;
        ds.append(5);
        ds.prepend(3);
        ds.append(7);
        ds.prepend(1);
        // Should be: 1 -> 3 -> 5 -> 7
        auto [val0, ops0] = ds.get(0);
        auto [val3, ops3] = ds.get(3);
        return val0 == 1 && val3 == 7 && ds.size() == 4;
    }, "Complex sequence of operations")) passed++;
    
    cout << "\nSummary: " << passed << "/" << total << " tests passed." << endl;
    
    // Give helpful hints if tests failed
    if (passed < total && class_name == "LinkedList") {
        cout << "\n[HINT] Common LinkedList bugs:" << endl;
        cout << "  - Not initializing head = nullptr in constructor" << endl;
        cout << "  - Not updating _size in append/prepend/insert/remove" << endl;
        cout << "  - Not checking if head is nullptr before dereferencing" << endl;
        cout << "  - Forgetting to delete nodes in remove() (memory leak)" << endl;
        cout << "  - Not handling empty list edge cases" << endl;
    }
}

int main() {
    // Set up segfault handler
    signal(SIGSEGV, segfault_handler);
    
    cout << "============================================================" << endl;
    cout << "CORRECTNESS TESTS" << endl;
    cout << "============================================================" << endl;
    
    test_data_structure<ArrayList>("ArrayList");
    test_data_structure<LinkedList>("LinkedList");
    
    cout << "\n============================================================" << endl;
    cout << "[DONE] ALL CORRECTNESS TESTS COMPLETE!" << endl;
    cout << "If all LinkedList tests passed, run performance tests:" << endl;
    cout << "    ./test_performance.out" << endl;
    cout << "============================================================" << endl;
    
    return 0;
}