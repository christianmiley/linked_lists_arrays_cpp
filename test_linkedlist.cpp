/*
 * test_linkedlist.cpp
 * 
 * Progressive LinkedList tests that build on each other.
 * Tests use display() to verify contents without relying on other unimplemented methods.
 * 
 * Order: append -> get -> prepend -> insert_at -> search -> remove
 * 
 * Each test is independent and shows expected vs actual output for debugging.
 */

#include "ArrayList.h"
#include "LinkedList.h"
#include <iostream>
#include <string>
#include <functional>
#include <csignal>

using namespace std;

// Global flag for crash detection
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

// Test helper that shows expected vs actual
bool run_test(function<bool()> test_func, const string& description, const string& expected = "", const string& actual = "") {
    current_test_name = description;
    
    try {
        bool result = test_func();
        if (result) {
            cout << "[✓] " << description << endl;
            return true;
        } else {
            cout << "[✗] " << description << endl;
            if (!expected.empty() && !actual.empty()) {
                cout << "    Expected: " << expected << endl;
                cout << "    Actual:   " << actual << endl;
            }
            return false;
        }
    } catch (const std::exception& e) {
        cout << "[ERROR] " << description << " — exception: " << e.what() << endl;
        return false;
    } catch (...) {
        cout << "[ERROR] " << description << " — unknown exception" << endl;
        return false;
    }
}

int main() {
    // Set up segfault handler
    signal(SIGSEGV, segfault_handler);
    
    cout << "============================================================" << endl;
    cout << "LINKEDLIST CORRECTNESS TESTS" << endl;
    cout << "============================================================" << endl;
    cout << "\nTests are run in order of dependency." << endl;
    cout << "Fix errors before proceeding to the next section.\n" << endl;
    
    int total_passed = 0;
    int total_tests = 0;
    
    // ========================================================================
    // SECTION 1: APPEND (Foundation - uses display() to verify)
    // ========================================================================
    cout << "--- Section 1: APPEND (Foundation) ---" << endl;
    int section_passed = 0;
    int section_total = 0;
    
    // Test 1.1: Append to empty list
    section_total++; total_tests++;
    {
        LinkedList ll;
        ll.append(42);
        string expected = "[42]";
        string actual = ll.display();
        if (run_test([&]() {
            return actual == expected && ll.size() == 1;
        }, "Append to empty list", expected, actual)) {
            section_passed++; total_passed++;
        }
    }
    
    // Test 1.2: Append two elements
    section_total++; total_tests++;
    {
        LinkedList ll;
        ll.append(10);
        ll.append(20);
        string expected = "[10 -> 20]";
        string actual = ll.display();
        if (run_test([&]() {
            return actual == expected && ll.size() == 2;
        }, "Append two elements", expected, actual)) {
            section_passed++; total_passed++;
        }
    }
    
    // Test 1.3: Append multiple elements
    section_total++; total_tests++;
    {
        LinkedList ll;
        ll.append(5);
        ll.append(10);
        ll.append(15);
        ll.append(20);
        string expected = "[5 -> 10 -> 15 -> 20]";
        string actual = ll.display();
        if (run_test([&]() {
            return actual == expected && ll.size() == 4;
        }, "Append multiple elements", expected, actual)) {
            section_passed++; total_passed++;
        }
    }
    
    // Test 1.4: Append maintains order
    section_total++; total_tests++;
    {
        LinkedList ll;
        for (int i = 1; i <= 5; i++) {
            ll.append(i);
        }
        string expected = "[1 -> 2 -> 3 -> 4 -> 5]";
        string actual = ll.display();
        if (run_test([&]() {
            return actual == expected && ll.size() == 5;
        }, "Append maintains correct order", expected, actual)) {
            section_passed++; total_passed++;
        }
    }
    
    cout << "Section 1: " << section_passed << "/" << section_total << " passed\n" << endl;
    
    if (section_passed < section_total) {
        cout << "[!] Fix append() before proceeding. All other tests depend on it.\n" << endl;
        cout << "[HINT] Common append() bugs:" << endl;
        cout << "  - Not updating _size (check: _size++ at end of function)" << endl;
        cout << "  - Not handling empty list: if (!head) { head = new_node; ... }" << endl;
        cout << "  - Not linking nodes: current->next = new_node" << endl;
        cout << "  - Not traversing to end: while (current->next) { current = current->next; }" << endl;
        return 1;
    }
    
    // ========================================================================
    // SECTION 2: GET (Now that append works, we can test get)
    // ========================================================================
    cout << "--- Section 2: GET ---" << endl;
    section_passed = 0;
    section_total = 0;
    
    // Test 2.1: Get from empty list
    section_total++; total_tests++;
    if (run_test([]() {
        LinkedList ll;
        auto [val, ops] = ll.get(0);
        return val == -1;
    }, "Get from empty list returns -1")) {
        section_passed++; total_passed++;
    }
    
    // Test 2.2: Get out of bounds
    section_total++; total_tests++;
    if (run_test([]() {
        LinkedList ll;
        ll.append(10);
        ll.append(20);
        auto [val, ops] = ll.get(5);
        return val == -1;
    }, "Get out of bounds returns -1")) {
        section_passed++; total_passed++;
    }
    
    // Test 2.3: Get first element
    section_total++; total_tests++;
    if (run_test([]() {
        LinkedList ll;
        ll.append(100);
        ll.append(200);
        auto [val, ops] = ll.get(0);
        return val == 100;
    }, "Get first element (index 0)")) {
        section_passed++; total_passed++;
    }
    
    // Test 2.4: Get middle element
    section_total++; total_tests++;
    if (run_test([]() {
        LinkedList ll;
        ll.append(10);
        ll.append(20);
        ll.append(30);
        auto [val, ops] = ll.get(1);
        return val == 20;
    }, "Get middle element")) {
        section_passed++; total_passed++;
    }
    
    // Test 2.5: Get last element
    section_total++; total_tests++;
    if (run_test([]() {
        LinkedList ll;
        ll.append(5);
        ll.append(10);
        ll.append(15);
        auto [val, ops] = ll.get(2);
        return val == 15;
    }, "Get last element")) {
        section_passed++; total_passed++;
    }
    
    // Test 2.6: Get from longer list
    section_total++; total_tests++;
    if (run_test([]() {
        LinkedList ll;
        for (int i = 0; i < 10; i++) {
            ll.append(i * 10);
        }
        auto [val0, ops0] = ll.get(0);
        auto [val5, ops5] = ll.get(5);
        auto [val9, ops9] = ll.get(9);
        return val0 == 0 && val5 == 50 && val9 == 90;
    }, "Get from various positions in longer list")) {
        section_passed++; total_passed++;
    }
    
    cout << "Section 2: " << section_passed << "/" << section_total << " passed\n" << endl;
    
    if (section_passed < section_total) {
        cout << "[!] Fix get() before proceeding.\n" << endl;
        cout << "[HINT] Common get() bugs:" << endl;
        cout << "  - Not checking bounds: if (index < 0 || index >= _size) return {-1, 0}" << endl;
        cout << "  - Not traversing correct number of times" << endl;
        cout << "  - Off-by-one errors in loop" << endl;
        return 1;
    }
    
    // ========================================================================
    // SECTION 3: PREPEND (Uses display() to verify)
    // ========================================================================
    cout << "--- Section 3: PREPEND ---" << endl;
    section_passed = 0;
    section_total = 0;
    
    // Test 3.1: Prepend to empty list
    section_total++; total_tests++;
    {
        LinkedList ll;
        ll.prepend(99);
        string expected = "[99]";
        string actual = ll.display();
        if (run_test([&]() {
            return actual == expected && ll.size() == 1;
        }, "Prepend to empty list", expected, actual)) {
            section_passed++; total_passed++;
        }
    }
    
    // Test 3.2: Prepend to non-empty list
    section_total++; total_tests++;
    {
        LinkedList ll;
        ll.append(20);
        ll.prepend(10);
        string expected = "[10 -> 20]";
        string actual = ll.display();
        if (run_test([&]() {
            return actual == expected && ll.size() == 2;
        }, "Prepend to non-empty list", expected, actual)) {
            section_passed++; total_passed++;
        }
    }
    
    // Test 3.3: Multiple prepends
    section_total++; total_tests++;
    {
        LinkedList ll;
        ll.prepend(30);
        ll.prepend(20);
        ll.prepend(10);
        string expected = "[10 -> 20 -> 30]";
        string actual = ll.display();
        if (run_test([&]() {
            return actual == expected && ll.size() == 3;
        }, "Multiple prepends", expected, actual)) {
            section_passed++; total_passed++;
        }
    }
    
    // Test 3.4: Mix append and prepend
    section_total++; total_tests++;
    {
        LinkedList ll;
        ll.append(30);
        ll.prepend(10);
        ll.append(40);
        ll.prepend(5);
        string expected = "[5 -> 10 -> 30 -> 40]";
        string actual = ll.display();
        if (run_test([&]() {
            return actual == expected && ll.size() == 4;
        }, "Mix of append and prepend", expected, actual)) {
            section_passed++; total_passed++;
        }
    }
    
    cout << "Section 3: " << section_passed << "/" << section_total << " passed\n" << endl;
    
    if (section_passed < section_total) {
        cout << "[!] Fix prepend() before proceeding.\n" << endl;
        cout << "[HINT] Common prepend() bugs:" << endl;
        cout << "  - Not updating _size" << endl;
        cout << "  - Not setting new_node->next = head BEFORE updating head" << endl;
        cout << "  - Not handling empty list case" << endl;
        return 1;
    }
    
    // ========================================================================
    // SECTION 4: INSERT_AT (Uses display() to verify)
    // ========================================================================
    cout << "--- Section 4: INSERT_AT ---" << endl;
    section_passed = 0;
    section_total = 0;
    
    // Test 4.1: Insert at beginning (index 0)
    section_total++; total_tests++;
    {
        LinkedList ll;
        ll.append(20);
        ll.append(30);
        ll.insert_at(0, 10);
        string expected = "[10 -> 20 -> 30]";
        string actual = ll.display();
        if (run_test([&]() {
            return actual == expected && ll.size() == 3;
        }, "Insert at index 0 (beginning)", expected, actual)) {
            section_passed++; total_passed++;
        }
    }
    
    // Test 4.2: Insert in middle
    section_total++; total_tests++;
    {
        LinkedList ll;
        ll.append(10);
        ll.append(30);
        ll.insert_at(1, 20);
        string expected = "[10 -> 20 -> 30]";
        string actual = ll.display();
        if (run_test([&]() {
            return actual == expected && ll.size() == 3;
        }, "Insert in middle", expected, actual)) {
            section_passed++; total_passed++;
        }
    }
    
    // Test 4.3: Insert at end
    section_total++; total_tests++;
    {
        LinkedList ll;
        ll.append(10);
        ll.append(20);
        ll.insert_at(2, 30);
        string expected = "[10 -> 20 -> 30]";
        string actual = ll.display();
        if (run_test([&]() {
            return actual == expected && ll.size() == 3;
        }, "Insert at end", expected, actual)) {
            section_passed++; total_passed++;
        }
    }
    
    // Test 4.4: Multiple inserts
    section_total++; total_tests++;
    {
        LinkedList ll;
        ll.append(10);
        ll.append(40);
        ll.insert_at(1, 20);
        ll.insert_at(2, 30);
        string expected = "[10 -> 20 -> 30 -> 40]";
        string actual = ll.display();
        if (run_test([&]() {
            return actual == expected && ll.size() == 4;
        }, "Multiple inserts at different positions", expected, actual)) {
            section_passed++; total_passed++;
        }
    }
    
    cout << "Section 4: " << section_passed << "/" << section_total << " passed\n" << endl;
    
    if (section_passed < section_total) {
        cout << "[!] Fix insert_at() before proceeding.\n" << endl;
        cout << "[HINT] Common insert_at() bugs:" << endl;
        cout << "  - Not updating _size" << endl;
        cout << "  - Off-by-one errors (should traverse to index-1, then insert after)" << endl;
        cout << "  - Not handling index 0 case (should use prepend or similar logic)" << endl;
        cout << "  - Not properly linking: new_node->next = current->next; current->next = new_node" << endl;
        return 1;
    }
    
    // ========================================================================
    // SECTION 5: SEARCH (Uses get() to set up, independent verification)
    // ========================================================================
    cout << "--- Section 5: SEARCH ---" << endl;
    section_passed = 0;
    section_total = 0;
    
    // Test 5.1: Search in empty list
    section_total++; total_tests++;
    if (run_test([]() {
        LinkedList ll;
        auto [idx, ops] = ll.search(10);
        return idx == -1;
    }, "Search in empty list returns -1")) {
        section_passed++; total_passed++;
    }
    
    // Test 5.2: Search for first element
    section_total++; total_tests++;
    if (run_test([]() {
        LinkedList ll;
        ll.append(10);
        ll.append(20);
        ll.append(30);
        auto [idx, ops] = ll.search(10);
        return idx == 0;
    }, "Search finds first element")) {
        section_passed++; total_passed++;
    }
    
    // Test 5.3: Search for middle element
    section_total++; total_tests++;
    if (run_test([]() {
        LinkedList ll;
        ll.append(10);
        ll.append(20);
        ll.append(30);
        auto [idx, ops] = ll.search(20);
        return idx == 1;
    }, "Search finds middle element")) {
        section_passed++; total_passed++;
    }
    
    // Test 5.4: Search for last element
    section_total++; total_tests++;
    if (run_test([]() {
        LinkedList ll;
        ll.append(10);
        ll.append(20);
        ll.append(30);
        auto [idx, ops] = ll.search(30);
        return idx == 2;
    }, "Search finds last element")) {
        section_passed++; total_passed++;
    }
    
    // Test 5.5: Search for non-existing element
    section_total++; total_tests++;
    if (run_test([]() {
        LinkedList ll;
        ll.append(10);
        ll.append(20);
        auto [idx, ops] = ll.search(99);
        return idx == -1;
    }, "Search for non-existing element returns -1")) {
        section_passed++; total_passed++;
    }
    
    cout << "Section 5: " << section_passed << "/" << section_total << " passed\n" << endl;
    
    if (section_passed < section_total) {
        cout << "[!] Fix search() before proceeding.\n" << endl;
        cout << "[HINT] Common search() bugs:" << endl;
        cout << "  - Not returning correct index (should track index as you traverse)" << endl;
        cout << "  - Not returning -1 when not found" << endl;
        cout << "  - Not handling empty list" << endl;
        return 1;
    }
    
    // ========================================================================
    // SECTION 6: REMOVE (Uses display() to verify)
    // ========================================================================
    cout << "--- Section 6: REMOVE ---" << endl;
    section_passed = 0;
    section_total = 0;
    
    // Test 6.1: Remove from empty list
    section_total++; total_tests++;
    if (run_test([]() {
        LinkedList ll;
        auto [success, ops] = ll.remove(10);
        return !success && ll.size() == 0;
    }, "Remove from empty list returns false")) {
        section_passed++; total_passed++;
    }
    
    // Test 6.2: Remove non-existing element
    section_total++; total_tests++;
    {
        LinkedList ll;
        ll.append(10);
        ll.append(20);
        auto [success, ops] = ll.remove(99);
        string expected = "[10 -> 20]";
        string actual = ll.display();
        if (run_test([&]() {
            return !success && actual == expected && ll.size() == 2;
        }, "Remove non-existing element returns false", expected, actual)) {
            section_passed++; total_passed++;
        }
    }
    
    // Test 6.3: Remove first element
    section_total++; total_tests++;
    {
        LinkedList ll;
        ll.append(10);
        ll.append(20);
        ll.append(30);
        auto [success, ops] = ll.remove(10);
        string expected = "[20 -> 30]";
        string actual = ll.display();
        if (run_test([&]() {
            return success && actual == expected && ll.size() == 2;
        }, "Remove first element", expected, actual)) {
            section_passed++; total_passed++;
        }
    }
    
    // Test 6.4: Remove middle element
    section_total++; total_tests++;
    {
        LinkedList ll;
        ll.append(10);
        ll.append(20);
        ll.append(30);
        auto [success, ops] = ll.remove(20);
        string expected = "[10 -> 30]";
        string actual = ll.display();
        if (run_test([&]() {
            return success && actual == expected && ll.size() == 2;
        }, "Remove middle element", expected, actual)) {
            section_passed++; total_passed++;
        }
    }
    
    // Test 6.5: Remove last element
    section_total++; total_tests++;
    {
        LinkedList ll;
        ll.append(10);
        ll.append(20);
        ll.append(30);
        auto [success, ops] = ll.remove(30);
        string expected = "[10 -> 20]";
        string actual = ll.display();
        if (run_test([&]() {
            return success && actual == expected && ll.size() == 2;
        }, "Remove last element", expected, actual)) {
            section_passed++; total_passed++;
        }
    }
    
    // Test 6.6: Remove only element
    section_total++; total_tests++;
    {
        LinkedList ll;
        ll.append(42);
        auto [success, ops] = ll.remove(42);
        string expected = "[]";
        string actual = ll.display();
        if (run_test([&]() {
            return success && actual == expected && ll.size() == 0;
        }, "Remove only element (list becomes empty)", expected, actual)) {
            section_passed++; total_passed++;
        }
    }
    
    cout << "Section 6: " << section_passed << "/" << section_total << " passed\n" << endl;
    
    if (section_passed < section_total) {
        cout << "[!] Fix remove() before proceeding.\n" << endl;
        cout << "[HINT] Common remove() bugs:" << endl;
        cout << "  - Not updating _size" << endl;
        cout << "  - Not deleting the removed node: delete temp; (memory leak!)" << endl;
        cout << "  - Not handling head removal: head = head->next" << endl;
        cout << "  - Not properly unlinking: current->next = current->next->next" << endl;
        return 1;
    }
    
    // ========================================================================
    // FINAL SUMMARY
    // ========================================================================
    cout << "\n============================================================" << endl;
    cout << "FINAL RESULTS" << endl;
    cout << "============================================================" << endl;
    cout << "Total: " << total_passed << "/" << total_tests << " tests passed" << endl;
    
    if (total_passed == total_tests) {
        cout << "\n[SUCCESS] All tests passed! Your LinkedList is working correctly." << endl;
        cout << "\nNext steps:" << endl;
        cout << "  1. Run performance tests: ./test_performance.out" << endl;
        cout << "  2. Try the extension challenge (tail pointer optimization)" << endl;
        cout << "============================================================" << endl;
        return 0;
    } else {
        cout << "\n[INCOMPLETE] Some tests failed. Review the errors above." << endl;
        cout << "============================================================" << endl;
        return 1;
    }
}