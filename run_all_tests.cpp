#include <iostream>
#include <cstdlib>

using namespace std;

int main() {
    cout << R"(
    ╔═══════════════════════════════════════════════════════════╗
    ║     DATA STRUCTURES PERFORMANCE COMPARISON                ║
    ║     ArrayList vs LinkedList                               ║
    ╚═══════════════════════════════════════════════════════════╝
    )" << endl;
    
    // Run correctness tests
    cout << "Running correctness tests..." << endl;
    int correctness_result = system("./test_correctness");
    
    if (correctness_result != 0) {
        cout << "\n⚠️  Correctness tests failed. Please fix errors before continuing." << endl;
        return 1;
    }
    
    // Ask before running performance tests
    cout << "\n✅ Correctness tests passed! Run performance tests? (y/n): ";
    char response;
    cin >> response;
    
    if (response != 'y' && response != 'Y') {
        cout << "Performance tests skipped." << endl;
        return 0;
    }
    
    // Run performance tests
    system("./test_performance");
    
    cout << "\n🎉 All tests complete!" << endl;
    
    return 0;
}