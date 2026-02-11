#include "ArrayList.h"
#include "LinkedList.h"
#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <fstream>
#include <iomanip>

using namespace std;

const vector<int> SIZES = {100, 500, 1000, 2000, 5000};

struct BenchmarkResult {
    int size;
    int array_ops;
    double array_time;
    int linked_ops;
    double linked_time;
};

template<typename T>
pair<int, double> benchmark_operation(const string& operation, int size) {
    T ds;
    int total_ops = 0;
    
    auto start = chrono::high_resolution_clock::now();
    
    if (operation == "append") {
        for (int i = 0; i < size; i++) {
            total_ops += ds.append(i);
        }
    } else if (operation == "prepend") {
        for (int i = 0; i < size; i++) {
            total_ops += ds.prepend(i);
        }
    } else if (operation == "get") {
        // Populate list first
        for (int i = 0; i < size; i++) {
            ds.append(i);
        }
        // Random gets
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(0, size - 1);
        for (int i = 0; i < 100; i++) {
            auto [val, ops] = ds.get(dis(gen));
            total_ops += ops;
        }
    } else if (operation == "search") {
        // Populate list
        for (int i = 0; i < size; i++) {
            ds.append(i);
        }
        // Search for last element (worst case)
        auto [idx, ops] = ds.search(size - 1);
        total_ops = ops;
    } else if (operation == "insert_middle") {
        // Populate list first
        for (int i = 0; i < size; i++) {
            ds.append(i);
        }
        // Insert at middle
        total_ops = ds.insert_at(size / 2, -1);
    }
    
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    
    return {total_ops, elapsed.count()};
}

vector<BenchmarkResult> run_challenge(const string& challenge_name, const string& operation) {
    cout << "\n============================================================" << endl;
    cout << "CHALLENGE: " << challenge_name << endl;
    cout << "============================================================" << endl;
    
    vector<BenchmarkResult> results;
    
    for (int size : SIZES) {
        cout << "\nTesting with size " << size << "..." << endl;
        
        auto [array_ops, array_time] = benchmark_operation<ArrayList>(operation, size);
        cout << "  ArrayList:  " << setw(10) << array_ops << " operations, " 
             << fixed << setprecision(6) << array_time << "s" << endl;
        
        auto [linked_ops, linked_time] = benchmark_operation<LinkedList>(operation, size);
        cout << "  LinkedList: " << setw(10) << linked_ops << " operations, " 
             << fixed << setprecision(6) << linked_time << "s" << endl;
        
        results.push_back({size, array_ops, array_time, linked_ops, linked_time});
    }
    
    return results;
}

void save_results_for_plotting(const string& filename, const vector<BenchmarkResult>& results) {
    ofstream file(filename);
    file << "size,array_ops,array_time,linked_ops,linked_time\n";
    for (const auto& r : results) {
        file << r.size << "," << r.array_ops << "," << r.array_time << ","
             << r.linked_ops << "," << r.linked_time << "\n";
    }
    file.close();
}

void print_analysis(const string& operation) {
    cout << "\n────────────────────────────────────────────────────────────" << endl;
    cout << "ANALYSIS:" << endl;
    
    if (operation == "search") {
        cout << "  🤝 TIE: Both are O(n)" << endl;
        cout << "     Why? Both must check each element until found." << endl;
        cout << "     (ArrayList slightly faster due to cache locality)" << endl;
    } else if (operation == "insert_middle") {
        cout << "  🤝 TIE: Both are O(n), but for different reasons" << endl;
        cout << "     ArrayList: O(n) to shift elements after insertion point" << endl;
        cout << "     LinkedList: O(n) to traverse to insertion point" << endl;
    } else if (operation == "prepend") {
        cout << "  🏆 LinkedList WINS: O(1) prepend vs ArrayList's O(n)" << endl;
        cout << "     Why? LinkedList just updates head pointer." << endl;
        cout << "     ArrayList must shift ALL elements to the right." << endl;
    } else if (operation == "get") {
        cout << "  🏆 ArrayList WINS: O(1) index access vs LinkedList's O(n)" << endl;
        cout << "     Why? ArrayList can jump directly to any index." << endl;
        cout << "     LinkedList must traverse from the head." << endl;
    } else if (operation == "append") {
        cout << "  🏆 ArrayList WINS: O(1) append vs LinkedList's O(n)" << endl;
        cout << "     Why? ArrayList just adds to the end." << endl;
        cout << "     LinkedList must traverse to find the end." << endl;
        cout << "     💡 NOTE: This could be O(1) with a tail pointer!" << endl;
    }
    
    cout << "────────────────────────────────────────────────────────────" << endl;
}

int main() {
    cout << "\n============================================================" << endl;
    cout << "PERFORMANCE TESTS" << endl;
    cout << "============================================================" << endl;
    cout << "\nYou'll run through 5 challenges comparing ArrayList vs LinkedList." << endl;
    cout << "After each challenge, you'll see analysis." << endl;
    cout << "Results will be saved to CSV files for plotting.\n" << endl;
    
    vector<pair<string, string>> challenges = {
        {"Challenge 1: Search for Element", "search"},
        {"Challenge 2: Insert at Middle", "insert_middle"},
        {"Challenge 3: Insert at Beginning (Prepend)", "prepend"},
        {"Challenge 4: Random Access by Index", "get"},
        {"Challenge 5: Append to End", "append"}
    };
    
    for (size_t i = 0; i < challenges.size(); i++) {
        auto [challenge_name, operation] = challenges[i];
        
        auto results = run_challenge(challenge_name, operation);
        print_analysis(operation);
        
        // Save results for plotting
        string filename = "challenge_" + to_string(i + 1) + "_results.csv";
        save_results_for_plotting(filename, results);
        cout << "\n💾 Results saved to: " << filename << endl;
        
        if (i < challenges.size() - 1) {
            cout << "\n✓ Challenge " << (i + 1) << " complete. Press Enter to continue to Challenge " 
                 << (i + 2) << "...";
            cin.ignore();
            cin.get();
        }
    }
    
    // Final summary
    cout << "\n============================================================" << endl;
    cout << "ALL CHALLENGES COMPLETE!" << endl;
    cout << "============================================================" << endl;
    cout << "\n📊 KEY TAKEAWAYS:" << endl;
    cout << "============================================================" << endl;
    cout << "✓ ArrayList excels at: random access, appending to end" << endl;
    cout << "✓ LinkedList excels at: inserting/removing at front" << endl;
    cout << "✓ The Big Idea: Choose your data structure based on which" << endl;
    cout << "               operations you need to be fast!" << endl;
    cout << "============================================================" << endl;
    
    cout << "\n✓ All 5 challenges complete. Press Enter to see Extension Challenge...";
    cin.get();
    
    // Extension challenge instructions
    cout << "\n============================================================" << endl;
    cout << "🎯 EXTENSION CHALLENGE: Optimize LinkedList Append" << endl;
    cout << "============================================================" << endl;
    cout << "\nLinkedList lost Challenge 5 because append is O(n)—it has to" << endl;
    cout << "traverse to the end every time. But we can fix this!" << endl;
    cout << "" << endl;
    cout << "─── THE SOLUTION: Add a Tail Pointer ───" << endl;
    cout << "" << endl;
    cout << "Currently your LinkedList only tracks head (first node)." << endl;
    cout << "If we ALSO track tail (last node), we can jump to the" << endl;
    cout << "end instantly, making append O(1)!" << endl;
    cout << "" << endl;
    cout << "─── YOUR TASK ───" << endl;
    cout << "" << endl;
    cout << "Modify your LinkedList class in LinkedList.h and LinkedList.cpp:" << endl;
    cout << "" << endl;
    cout << "STEP 1: In LinkedList.h, add to private members:" << endl;
    cout << "    Node* tail;" << endl;
    cout << "" << endl;
    cout << "STEP 2: Update append() to use the tail pointer:" << endl;
    cout << "    - Create the new node" << endl;
    cout << "    - If the list is empty (no head):" << endl;
    cout << "        Set both head AND tail to the new node" << endl;
    cout << "    - If the list has elements:" << endl;
    cout << "        Link the current tail's next to the new node" << endl;
    cout << "        Update tail to point to the new node" << endl;
    cout << "    - Update size and return 1 (now O(1)!)" << endl;
    cout << "" << endl;
    cout << "STEP 3: Update prepend() to maintain tail:" << endl;
    cout << "    - After adding the new node as head..." << endl;
    cout << "    - If the list WAS empty (size was 0):" << endl;
    cout << "        The new node is also the tail" << endl;
    cout << "" << endl;
    cout << "STEP 4: Update remove() to maintain tail when deleting head:" << endl;
    cout << "    - After removing the head node..." << endl;
    cout << "    - If the list is NOW empty (no head):" << endl;
    cout << "        Set tail to nullptr" << endl;
    cout << "" << endl;
    cout << "STEP 5: Update remove() to maintain tail when deleting last node:" << endl;
    cout << "    - When you find and remove a node..." << endl;
    cout << "    - If the node you just removed was the last one:" << endl;
    cout << "        (Hint: check if current->next is nullptr)" << endl;
    cout << "        Update tail to point to current" << endl;
    cout << "" << endl;
    cout << "─── TEST YOUR OPTIMIZATION ───" << endl;
    cout << "" << endl;
    cout << "Once you've made these changes, run:" << endl;
    cout << "    ./test_tail_optimization" << endl;
    cout << "" << endl;
    cout << "You should see append performance improve dramatically!" << endl;
    cout << "Before: O(n²) total operations" << endl;
    cout << "After:  O(n) total operations" << endl;
    cout << "" << endl;
    cout << "============================================================" << endl;
    
    return 0;
}