/*
 * test_linkedlist.cpp
 *
 * Progressive LinkedList tests that build on each other.
 *
 * IMPORTANT: These tests are NOT fully independent. Sections run in dependency
 * order and every section after APPEND uses append() to build its test lists,
 * so a broken append() stops the run early (by design).
 *
 * Two distinct properties are verified SEPARATELY so failures are unambiguous:
 *   - CONTENTS: the actual node chain, via display() (pointer-based; ignores _size)
 *   - SIZE:     the size() counter (_size), checked in dedicated size tests
 * Keeping these apart means a wrong _size shows up as a clear size failure
 * instead of silently failing a contents test whose output looks correct.
 *
 * Order: append -> get -> prepend -> insert_at -> search -> remove
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
    cout << "[CRASH] " << current_test_name << " - SEGMENTATION FAULT" << endl;
    cout << "        This usually means:" << endl;
    cout << "        - Dereferencing a null pointer (e.g., current->next when current is nullptr)" << endl;
    cout << "        - Using deleted memory" << endl;
    cout << "        - Accessing array out of bounds" << endl;
    exit(1);
}

bool run_cycle_check(const LinkedList& ll, const std::string& description) {
    current_test_name = description;

    if (ll.has_cycle()) {
        cout << "[X] " << description << " - cycle detected" << endl;
        cout << "    This usually means a node's next pointer was linked incorrectly." << endl;
        return false;
    } else {
        cout << "[OK] " << description << " - no cycle detected" << endl;
        return true;
    }
}

// ---- Reporting helpers -----------------------------------------------------
// Each helper checks ONE property and prints a clear pass/fail line, so that
// (for example) a wrong _size is reported as a size failure rather than masked
// behind a contents test whose displayed output looks identical.

// Boolean test helper for checks that return a single value (e.g. search/get).
bool run_test(function<bool()> test_func, const string& description, const string& expected = "", const string& actual = "") {
    current_test_name = description;

    try {
        bool result = test_func();
        if (result) {
            cout << "[OK] " << description << endl;
            return true;
        } else {
            cout << "[X] " << description << endl;
            if (!expected.empty() && !actual.empty()) {
                cout << "    Expected: " << expected << endl;
                cout << "    Actual:   " << actual << endl;
            }
            return false;
        }
    } catch (const std::exception& e) {
        cout << "[ERROR] " << description << " - exception: " << e.what() << endl;
        return false;
    } catch (...) {
        cout << "[ERROR] " << description << " - unknown exception" << endl;
        return false;
    }
}

// CONTENTS only: compares the actual node chain via display() (pointer-based).
// Deliberately ignores size() so a _size bug cannot mask a structure pass.
bool check_contents(const LinkedList& ll, const string& expected, const string& description) {
    current_test_name = description;
    string actual = ll.display();
    if (actual == expected) {
        cout << "[OK] " << description << endl;
        return true;
    }
    cout << "[X] " << description << endl;
    cout << "    contents (expected): " << expected << endl;
    cout << "    contents (actual):   " << actual << endl;
    return false;
}

// SIZE only: compares size() against the expected count.
bool check_size(const LinkedList& ll, int expected, const string& description) {
    current_test_name = description;
    int actual = ll.size();
    if (actual == expected) {
        cout << "[OK] " << description << endl;
        return true;
    }
    cout << "[X] " << description << endl;
    cout << "    size() expected: " << expected << endl;
    cout << "    size() actual:   " << actual << "   <-- did you update _size?" << endl;
    return false;
}

// For operations that return a (success, ops) flag plus leave the list in a
// known state (insert_at, remove): checks the returned success flag AND the
// resulting contents (size is covered separately by dedicated size tests).
// Each failing dimension is reported independently.
bool check_result(bool success, bool expected_success,
                  const LinkedList& ll, const string& expected_contents,
                  const string& description) {
    current_test_name = description;
    bool success_ok = (success == expected_success);
    string actual = ll.display();
    bool contents_ok = (actual == expected_contents);
    if (success_ok && contents_ok) {
        cout << "[OK] " << description << endl;
        return true;
    }
    cout << "[X] " << description << endl;
    if (!success_ok) {
        cout << "    return value expected: " << (expected_success ? "true" : "false") << endl;
        cout << "    return value actual:   " << (success ? "true" : "false") << endl;
    }
    if (!contents_ok) {
        cout << "    contents (expected): " << expected_contents << endl;
        cout << "    contents (actual):   " << actual << endl;
    }
    return false;
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
    // SECTION 1: APPEND (Foundation)
    //   Contents tests (1.1-1.4) verify the node chain only and pass even if
    //   _size is wrong. Size tests (1.5-1.6) verify the _size counter directly.
    // ========================================================================
    cout << "--- Section 1: APPEND (Foundation) ---" << endl;
    int section_passed = 0;
    int section_total = 0;

    // Test 1.1: Append to empty list (contents)
    section_total++; total_tests++;
    {
        LinkedList ll;
        ll.append(42);
        if (check_contents(ll, "[42]", "Append to empty list (contents)")) {
            section_passed++; total_passed++;
        }
    }

    // Test 1.2: Append two elements (contents)
    section_total++; total_tests++;
    {
        LinkedList ll;
        ll.append(10);
        ll.append(20);
        if (check_contents(ll, "[10 -> 20]", "Append two elements (contents)")) {
            section_passed++; total_passed++;
        }
    }

    // Test 1.3: Append multiple elements (contents)
    section_total++; total_tests++;
    {
        LinkedList ll;
        ll.append(5);
        ll.append(10);
        ll.append(15);
        ll.append(20);
        if (check_contents(ll, "[5 -> 10 -> 15 -> 20]", "Append multiple elements (contents)")) {
            section_passed++; total_passed++;
        }
    }

    // Test 1.4: Append maintains order (contents)
    section_total++; total_tests++;
    {
        LinkedList ll;
        for (int i = 1; i <= 5; i++) {
            ll.append(i);
        }
        if (check_contents(ll, "[1 -> 2 -> 3 -> 4 -> 5]", "Append maintains correct order (contents)")) {
            section_passed++; total_passed++;
        }
    }

    // Test 1.5: size() after a single append (size)
    section_total++; total_tests++;
    {
        LinkedList ll;
        ll.append(99);
        if (check_size(ll, 1, "size() == 1 after one append")) {
            section_passed++; total_passed++;
        }
    }

    // Test 1.6: size() grows by one per append (size)
    section_total++; total_tests++;
    {
        LinkedList ll;
        ll.append(10);
        ll.append(20);
        ll.append(30);
        ll.append(40);
        if (check_size(ll, 4, "size() == 4 after four appends")) {
            section_passed++; total_passed++;
        }
    }

    cout << "Section 1: " << section_passed << "/" << section_total << " passed\n" << endl;

    if (section_passed < section_total) {
        cout << "[!] Fix append() before proceeding. All other tests depend on it.\n" << endl;
        cout << "[HINT] Common append() bugs:" << endl;
        cout << "  - Not updating _size (shows up as a size() test failure above)" << endl;
        cout << "  - Not handling empty list: if (!head) { head = new_node; ... }" << endl;
        cout << "  - Not linking nodes: current->next = new_node" << endl;
        cout << "  - Not traversing to end: while (current->next) { current = current->next; }" << endl;
        return 1;
    }

    // ========================================================================
    // SECTION 2: GET (uses append() to set up; verifies returned values)
    // ========================================================================
    cout << "--- Section 2: GET ---" << endl;
    section_passed = 0;
    section_total = 0;

    // Test 2.1: Get from empty list (out of bounds -> found == false)
    section_total++; total_tests++;
    {
        LinkedList ll;
        auto [found, val, ops] = ll.get(0);
        bool passed = (!found);
        if (!passed) {
            cout << "[X] Get from empty list reports not found" << endl;
            cout << "    List: " << ll.display() << endl;
            cout << "    Expected: found == false" << endl;
            cout << "    Actual:   found == true (value " << val << ")" << endl;
        } else {
            cout << "[OK] Get from empty list reports not found" << endl;
            section_passed++; total_passed++;
        }
    }

    // Test 2.2: Get out of bounds (index past end -> found == false)
    section_total++; total_tests++;
    {
        LinkedList ll;
        ll.append(10);
        ll.append(20);
        auto [found, val, ops] = ll.get(5);
        bool passed = (!found);
        if (!passed) {
            cout << "[X] Get out of bounds reports not found" << endl;
            cout << "    List: " << ll.display() << endl;
            cout << "    Requested index: 5 (size is " << ll.size() << ")" << endl;
            cout << "    Expected: found == false" << endl;
            cout << "    Actual:   found == true (value " << val << ")" << endl;
        } else {
            cout << "[OK] Get out of bounds reports not found" << endl;
            section_passed++; total_passed++;
        }
    }

    // Test 2.3: Get negative index (-> found == false)
    section_total++; total_tests++;
    {
        LinkedList ll;
        ll.append(10);
        ll.append(20);
        auto [found, val, ops] = ll.get(-1);
        bool passed = (!found);
        if (!passed) {
            cout << "[X] Get negative index reports not found" << endl;
            cout << "    List: " << ll.display() << endl;
            cout << "    Requested index: -1" << endl;
            cout << "    Expected: found == false" << endl;
            cout << "    Actual:   found == true (value " << val << ")" << endl;
        } else {
            cout << "[OK] Get negative index reports not found" << endl;
            section_passed++; total_passed++;
        }
    }

    // Test 2.4: Get first element
    section_total++; total_tests++;
    {
        LinkedList ll;
        ll.append(100);
        ll.append(200);
        auto [found, val, ops] = ll.get(0);
        bool passed = (found && val == 100);
        if (!passed) {
            cout << "[X] Get first element (index 0)" << endl;
            cout << "    List: " << ll.display() << endl;
            cout << "    Requested index: 0" << endl;
            cout << "    Expected: found == true, value 100" << endl;
            cout << "    Actual:   found == " << (found ? "true" : "false") << ", value " << val << endl;
        } else {
            cout << "[OK] Get first element (index 0)" << endl;
            section_passed++; total_passed++;
        }
    }

    // Test 2.5: Get middle element
    section_total++; total_tests++;
    {
        LinkedList ll;
        ll.append(10);
        ll.append(20);
        ll.append(30);
        auto [found, val, ops] = ll.get(1);
        bool passed = (found && val == 20);
        if (!passed) {
            cout << "[X] Get middle element" << endl;
            cout << "    List: " << ll.display() << endl;
            cout << "    Requested index: 1" << endl;
            cout << "    Expected: found == true, value 20" << endl;
            cout << "    Actual:   found == " << (found ? "true" : "false") << ", value " << val << endl;
        } else {
            cout << "[OK] Get middle element" << endl;
            section_passed++; total_passed++;
        }
    }

    // Test 2.6: Get last element
    section_total++; total_tests++;
    {
        LinkedList ll;
        ll.append(5);
        ll.append(10);
        ll.append(15);
        auto [found, val, ops] = ll.get(2);
        bool passed = (found && val == 15);
        if (!passed) {
            cout << "[X] Get last element" << endl;
            cout << "    List: " << ll.display() << endl;
            cout << "    Requested index: 2" << endl;
            cout << "    Expected: found == true, value 15" << endl;
            cout << "    Actual:   found == " << (found ? "true" : "false") << ", value " << val << endl;
        } else {
            cout << "[OK] Get last element" << endl;
            section_passed++; total_passed++;
        }
    }

    // Test 2.7: Get a stored -1 (the found flag must distinguish it from OOB)
    section_total++; total_tests++;
    {
        LinkedList ll;
        ll.append(-1);
        auto [found, val, ops] = ll.get(0);
        bool passed = (found && val == -1);
        if (!passed) {
            cout << "[X] Get a stored -1 value" << endl;
            cout << "    List: " << ll.display() << endl;
            cout << "    Requested index: 0" << endl;
            cout << "    Expected: found == true, value -1" << endl;
            cout << "    Actual:   found == " << (found ? "true" : "false") << ", value " << val << endl;
        } else {
            cout << "[OK] Get a stored -1 value (found flag distinguishes it from out-of-bounds)" << endl;
            section_passed++; total_passed++;
        }
    }

    // Test 2.8: Get from longer list
    section_total++; total_tests++;
    {
        LinkedList ll;
        for (int i = 0; i < 10; i++) {
            ll.append(i * 10);
        }
        auto [found0, val0, ops0] = ll.get(0);
        auto [found5, val5, ops5] = ll.get(5);
        auto [found9, val9, ops9] = ll.get(9);
        bool passed = (found0 && val0 == 0 && found5 && val5 == 50 && found9 && val9 == 90);
        if (!passed) {
            cout << "[X] Get from various positions in longer list" << endl;
            cout << "    List: " << ll.display() << endl;
            cout << "    get(0): Expected 0, Got " << val0 << ((found0 && val0 == 0) ? " [OK]" : " [X]") << endl;
            cout << "    get(5): Expected 50, Got " << val5 << ((found5 && val5 == 50) ? " [OK]" : " [X]") << endl;
            cout << "    get(9): Expected 90, Got " << val9 << ((found9 && val9 == 90) ? " [OK]" : " [X]") << endl;
        } else {
            cout << "[OK] Get from various positions in longer list" << endl;
            section_passed++; total_passed++;
        }
    }

    cout << "Section 2: " << section_passed << "/" << section_total << " passed\n" << endl;

    if (section_passed < section_total) {
        cout << "[!] Fix get() before proceeding.\n" << endl;
        cout << "[HINT] Common get() bugs:" << endl;
        cout << "  - Not checking bounds: if (index < 0 || index >= _size) return {false, -1, 0}" << endl;
        cout << "  - Returning found == false for a valid index (or true when out of bounds)" << endl;
        cout << "  - Not traversing correct number of times" << endl;
        cout << "  - Off-by-one errors in loop" << endl;
        cout << "  - Returning wrong value (make sure to return current->data)" << endl;
        return 1;
    }

    // ========================================================================
    // SECTION 3: PREPEND
    //   Contents tests verify the node chain only; a dedicated size test
    //   verifies the _size counter.
    // ========================================================================
    cout << "--- Section 3: PREPEND ---" << endl;
    section_passed = 0;
    section_total = 0;

    // Test 3.1: Prepend to empty list (contents)
    section_total++; total_tests++;
    {
        LinkedList ll;
        ll.prepend(99);
        if (check_contents(ll, "[99]", "Prepend to empty list (contents)")) {
            section_passed++; total_passed++;
        }
    }

    // Test 3.2: Prepend to non-empty list (contents)
    section_total++; total_tests++;
    {
        LinkedList ll;
        ll.append(20);
        ll.prepend(10);
        if (check_contents(ll, "[10 -> 20]", "Prepend to non-empty list (contents)")) {
            section_passed++; total_passed++;
        }
    }

    // Test 3.3: Multiple prepends (contents)
    section_total++; total_tests++;
    {
        LinkedList ll;
        ll.prepend(30);
        ll.prepend(20);
        ll.prepend(10);
        if (check_contents(ll, "[10 -> 20 -> 30]", "Multiple prepends (contents)")) {
            section_passed++; total_passed++;
        }
    }

    // Test 3.4: Mix append and prepend (contents)
    section_total++; total_tests++;
    {
        LinkedList ll;
        ll.append(30);
        ll.prepend(10);
        ll.append(40);
        ll.prepend(5);
        if (check_contents(ll, "[5 -> 10 -> 30 -> 40]", "Mix of append and prepend (contents)")) {
            section_passed++; total_passed++;
        }
    }

    // Test 3.5: size() after prepends (size)
    section_total++; total_tests++;
    {
        LinkedList ll;
        ll.prepend(30);
        ll.prepend(20);
        ll.prepend(10);
        if (check_size(ll, 3, "size() == 3 after three prepends")) {
            section_passed++; total_passed++;
        }
    }

    cout << "Section 3: " << section_passed << "/" << section_total << " passed\n" << endl;

    if (section_passed < section_total) {
        cout << "[!] Fix prepend() before proceeding.\n" << endl;
        cout << "[HINT] Common prepend() bugs:" << endl;
        cout << "  - Not updating _size (shows up as a size() test failure above)" << endl;
        cout << "  - Not setting new_node->next = head BEFORE updating head" << endl;
        cout << "  - Not handling empty list case" << endl;
        return 1;
    }

    // ========================================================================
    // SECTION 4: INSERT_AT
    //   Contents tests verify the node chain only; a dedicated size test
    //   verifies the _size counter.
    // ========================================================================
    cout << "--- Section 4: INSERT_AT ---" << endl;
    section_passed = 0;
    section_total = 0;

    // Test 4.1: Insert at beginning (index 0) (return value + contents)
    section_total++; total_tests++;
    {
        LinkedList ll;
        ll.append(20);
        ll.append(30);
        auto [ok, ops] = ll.insert_at(0, 10);
        if (check_result(ok, true, ll, "[10 -> 20 -> 30]", "Insert at index 0 (beginning)")) {
            section_passed++; total_passed++;
        }
    }

    // Test 4.2: Insert in middle (return value + contents)
    section_total++; total_tests++;
    {
        LinkedList ll;
        ll.append(10);
        ll.append(30);
        auto [ok, ops] = ll.insert_at(1, 20);
        if (check_result(ok, true, ll, "[10 -> 20 -> 30]", "Insert in middle")) {
            section_passed++; total_passed++;
        }
    }

    // Test 4.3: Insert at end (index == size) (return value + contents)
    section_total++; total_tests++;
    {
        LinkedList ll;
        ll.append(10);
        ll.append(20);
        auto [ok, ops] = ll.insert_at(2, 30);
        if (check_result(ok, true, ll, "[10 -> 20 -> 30]", "Insert at end (index == size)")) {
            section_passed++; total_passed++;
        }
    }

    // Test 4.4: Multiple inserts (return value + contents)
    section_total++; total_tests++;
    {
        LinkedList ll;
        ll.append(10);
        ll.append(40);
        auto [ok1, ops1] = ll.insert_at(1, 20);
        auto [ok2, ops2] = ll.insert_at(2, 30);
        if (check_result(ok1 && ok2, true, ll, "[10 -> 20 -> 30 -> 40]", "Multiple inserts at different positions")) {
            section_passed++; total_passed++;
        }
    }

    // Test 4.5: Insert at negative index is rejected (return value + contents unchanged)
    section_total++; total_tests++;
    {
        LinkedList ll;
        ll.append(10);
        ll.append(20);
        auto [ok, ops] = ll.insert_at(-1, 99);
        if (check_result(ok, false, ll, "[10 -> 20]", "Insert at negative index returns false (list unchanged)")) {
            section_passed++; total_passed++;
        }
    }

    // Test 4.6: Insert past the end is rejected (return value + contents unchanged)
    section_total++; total_tests++;
    {
        LinkedList ll;
        ll.append(10);
        ll.append(20);
        auto [ok, ops] = ll.insert_at(5, 99);   // valid range is [0, 2]
        if (check_result(ok, false, ll, "[10 -> 20]", "Insert past end returns false (list unchanged)")) {
            section_passed++; total_passed++;
        }
    }

    // Test 4.7: size() after a valid insert (size)
    section_total++; total_tests++;
    {
        LinkedList ll;
        ll.append(10);
        ll.append(30);
        ll.insert_at(1, 20);   // size should now be 3
        if (check_size(ll, 3, "size() == 3 after two appends and one insert_at")) {
            section_passed++; total_passed++;
        }
    }

    // Test 4.8: size() unchanged after a rejected insert (size)
    section_total++; total_tests++;
    {
        LinkedList ll;
        ll.append(10);
        ll.append(20);
        ll.insert_at(5, 99);   // out of bounds: size must stay 2
        if (check_size(ll, 2, "size() == 2 after a rejected out-of-bounds insert (unchanged)")) {
            section_passed++; total_passed++;
        }
    }

    cout << "Section 4: " << section_passed << "/" << section_total << " passed\n" << endl;

    if (section_passed < section_total) {
        cout << "[!] Fix insert_at() before proceeding.\n" << endl;
        cout << "[HINT] Common insert_at() bugs:" << endl;
        cout << "  - Not checking bounds: reject index < 0 or index > _size, return {false, 0}" << endl;
        cout << "  - Changing the list (or _size) on a rejected out-of-bounds insert" << endl;
        cout << "  - Not updating _size (shows up as a size() test failure above)" << endl;
        cout << "  - Off-by-one errors (should traverse to index-1, then insert after)" << endl;
        cout << "  - Not handling index 0 case (should use prepend or similar logic)" << endl;
        cout << "  - Not properly linking: new_node->next = current->next; current->next = new_node" << endl;
        return 1;
    }

    // ========================================================================
    // SECTION 5: SEARCH (uses append() to set up; verifies returned index)
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
    // SECTION 6: REMOVE
    //   Contents/return-value tests verify the node chain and the success flag;
    //   dedicated size tests verify the _size counter (including that a FAILED
    //   remove does not change size).
    // ========================================================================
    cout << "--- Section 6: REMOVE ---" << endl;
    section_passed = 0;
    section_total = 0;

    // Test 6.1: Remove from empty list (return value + contents)
    section_total++; total_tests++;
    {
        LinkedList ll;
        auto [success, ops] = ll.remove(10);
        if (check_result(success, false, ll, "[]", "Remove from empty list returns false")) {
            section_passed++; total_passed++;
        }
    }

    // Test 6.2: Remove non-existing element (return value + contents)
    section_total++; total_tests++;
    {
        LinkedList ll;
        ll.append(10);
        ll.append(20);
        auto [success, ops] = ll.remove(99);
        if (check_result(success, false, ll, "[10 -> 20]", "Remove non-existing element returns false")) {
            section_passed++; total_passed++;
        }
    }

    // Test 6.3: Remove first element (return value + contents)
    section_total++; total_tests++;
    {
        LinkedList ll;
        ll.append(10);
        ll.append(20);
        ll.append(30);
        auto [success, ops] = ll.remove(10);
        if (check_result(success, true, ll, "[20 -> 30]", "Remove first element")) {
            section_passed++; total_passed++;
        }
    }

    // Test 6.4: Remove middle element (return value + contents)
    section_total++; total_tests++;
    {
        LinkedList ll;
        ll.append(10);
        ll.append(20);
        ll.append(30);
        auto [success, ops] = ll.remove(20);
        if (check_result(success, true, ll, "[10 -> 30]", "Remove middle element")) {
            section_passed++; total_passed++;
        }
    }

    // Test 6.5: Remove last element (return value + contents)
    section_total++; total_tests++;
    {
        LinkedList ll;
        ll.append(10);
        ll.append(20);
        ll.append(30);
        auto [success, ops] = ll.remove(30);
        if (check_result(success, true, ll, "[10 -> 20]", "Remove last element")) {
            section_passed++; total_passed++;
        }
    }

    // Test 6.6: Remove only element (return value + contents)
    section_total++; total_tests++;
    {
        LinkedList ll;
        ll.append(42);
        auto [success, ops] = ll.remove(42);
        if (check_result(success, true, ll, "[]", "Remove only element (list becomes empty)")) {
            section_passed++; total_passed++;
        }
    }

    // Test 6.7: size() decreases after a successful remove (size)
    section_total++; total_tests++;
    {
        LinkedList ll;
        ll.append(10);
        ll.append(20);
        ll.append(30);
        ll.remove(20);   // size should drop from 3 to 2
        if (check_size(ll, 2, "size() == 2 after removing one of three elements")) {
            section_passed++; total_passed++;
        }
    }

    // Test 6.8: size() unchanged after a failed remove (size)
    section_total++; total_tests++;
    {
        LinkedList ll;
        ll.append(10);
        ll.append(20);
        ll.remove(99);   // not found: size must stay 2
        if (check_size(ll, 2, "size() == 2 after a failed remove (unchanged)")) {
            section_passed++; total_passed++;
        }
    }

    cout << "Section 6: " << section_passed << "/" << section_total << " passed\n" << endl;

    if (section_passed < section_total) {
        cout << "[!] Fix remove() before proceeding.\n" << endl;
        cout << "[HINT] Common remove() bugs:" << endl;
        cout << "  - Not updating _size (shows up as a size() test failure above)" << endl;
        cout << "  - Decrementing _size even when the value was not found" << endl;
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
