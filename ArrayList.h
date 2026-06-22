#ifndef ARRAYLIST_H
#define ARRAYLIST_H

#include <vector>
#include <string>
#include <tuple>
#include <utility>

class ArrayList {
private:
    std::vector<int> _data;

public:
    ArrayList();

    // Operations that return operation count
    int append(int value);
    int prepend(int value);
    std::pair<bool, int> insert_at(int index, int value);  // Returns (success, operations)
    std::tuple<bool, int, int> get(int index);  // Returns (found, value, operations)
    std::pair<int, int> search(int value);  // Returns (index, operations)
    std::pair<bool, int> remove(int value);  // Returns (success, operations)
    
    int size() const;
    std::string display() const;
};

#endif