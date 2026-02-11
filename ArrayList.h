#ifndef ARRAYLIST_H
#define ARRAYLIST_H

#include <vector>
#include <string>

class ArrayList {
private:
    std::vector<int> _data;
    
public:
    ArrayList();
    
    // Operations that return operation count
    int append(int value);
    int prepend(int value);
    int insert_at(int index, int value);
    std::pair<int, int> get(int index);  // Returns (value, operations)
    std::pair<int, int> search(int value);  // Returns (index, operations)
    std::pair<bool, int> remove(int value);  // Returns (success, operations)
    
    int size() const;
    std::string display() const;
};

#endif