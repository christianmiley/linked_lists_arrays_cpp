#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <string>
#include <utility>
#include <tuple>

struct Node {
    int data;
    Node* next;
    
    Node(int val) : data(val), next(nullptr) {}
};

class LinkedList {
    private:
        Node* head;
        int _size;
        
    public:
        LinkedList();
        ~LinkedList();
        
        // Operations that return operation count
        int append(int value);
        int prepend(int value);
        std::pair<bool, int> insert_at(int index, int value);  // Returns (success, operations)
        std::tuple<bool, int, int> get(int index);  // Returns (found, value, operations)
        std::pair<int, int> search(int value);  // Returns (index, operations)
        std::pair<bool, int> remove(int value);  // Returns (success, operations)
        
        int size() const;
        std::string display() const;
        bool has_cycle() const;
};

#endif