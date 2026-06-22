#include "LinkedList.h"
#include <sstream>

LinkedList::LinkedList() : head(nullptr), _size(0) {}

LinkedList::~LinkedList() {
    // Clean up all nodes
    Node* current = head;
    while (current != nullptr) {
        Node* next = current->next;
        delete current;
        current = next;
    }
}

int LinkedList::append(int value) {
    /*
     * Add value to the end of the list.
     * Return number of operations (traversals + insertion).
     * 
     * Hint: You need to traverse to the end, then add a new node.
     * Count each node you visit as one operation.
     */
    // TODO: Implement this
    return 0;
}

int LinkedList::prepend(int value) {
    /*
     * Add value to the beginning of the list.
     * Return number of operations.
     * 
     * Hint: This should be O(1) - just update the head!
     */
    // TODO: Implement this
    return 0;
}

std::pair<bool, int> LinkedList::insert_at(int index, int value) {
    /*
     * Insert value at the given index (0-indexed).
     * Valid positions are [0, _size]; index == _size inserts at the end.
     * Return (true, operations) on success, or (false, 0) if the index is
     * out of bounds (index < 0 or index > _size).
     *
     * Hint: Check bounds first, then traverse to index-1 and insert after it.
     */
    // TODO: Implement this
    return {false, 0};
}

std::tuple<bool, int, int> LinkedList::get(int index) {
    /*
     * Get value at given index.
     * Return (true, value, operations) if the index is valid, or
     * (false, -1, operations) if out of bounds (index < 0 or index >= _size).
     *
     * Note: we return a found flag instead of using -1 as a sentinel, so that
     * -1 (and any other value) can be stored and retrieved unambiguously.
     *
     * Hint: Traverse from head, counting as you go.
     */
    // TODO: Implement this
    return {false, -1, 0};
}

std::pair<int, int> LinkedList::search(int value) {
    /*
     * Search for value in the list.
     * Return (index, operations) or (-1, operations) if not found.
     * 
     * Hint: Traverse and count operations until you find it or reach the end.
     */
    // TODO: Implement this
    return {-1, 0};
}

std::pair<bool, int> LinkedList::remove(int value) {
    /*
     * Delete the first occurrence of value.
     * Return (success, operations).
     * 
     * Hint: Find the node before the one to delete, then update its 'next' pointer.
     * Don't forget to delete the removed node to avoid memory leaks!
     */
    // TODO: Implement this
    return {false, 0};
}

int LinkedList::size() const {
    return _size;
}

std::string LinkedList::display() const {
    if (!head) {
        return "[]";
    }
    
    std::stringstream ss;
    ss << "[";
    Node* current = head;
    while (current) {
        ss << current->data;
        if (current->next) {
            ss << " -> ";
        }
        current = current->next;
    }
    ss << "]";
    return ss.str();
}

bool LinkedList::has_cycle() const {
    Node* slow = head;
    Node* fast = head;

    while (fast != nullptr && fast->next != nullptr) {
        slow = slow->next;
        fast = fast->next->next;

        if (slow == fast) {
            return true;
        }
    }

    return false;
}