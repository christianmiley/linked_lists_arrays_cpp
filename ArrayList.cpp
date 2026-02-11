#include "ArrayList.h"
#include <sstream>

ArrayList::ArrayList() {}

int ArrayList::append(int value) {
    _data.push_back(value);
    return 1;  // O(1) operation
}

int ArrayList::prepend(int value) {
    _data.insert(_data.begin(), value);
    return _data.size();  // O(n) - must shift all elements
}

int ArrayList::insert_at(int index, int value) {
    _data.insert(_data.begin() + index, value);
    // Operations = number of elements that need to shift
    return _data.size() - index;
}

std::pair<int, int> ArrayList::get(int index) {
    if (index < 0 || index >= static_cast<int>(_data.size())) {
        return {-1, 0};  // Return -1 for invalid index
    }
    return {_data[index], 1};  // O(1) operation
}

std::pair<int, int> ArrayList::search(int value) {
    int ops = 0;
    for (size_t i = 0; i < _data.size(); i++) {
        ops++;
        if (_data[i] == value) {
            return {static_cast<int>(i), ops};
        }
    }
    return {-1, ops};
}

std::pair<bool, int> ArrayList::remove(int value) {
    int ops = 0;
    for (size_t i = 0; i < _data.size(); i++) {
        ops++;
        if (_data[i] == value) {
            _data.erase(_data.begin() + i);
            ops += _data.size() - i;  // Cost of shifting elements
            return {true, ops};
        }
    }
    return {false, ops};
}

int ArrayList::size() const {
    return _data.size();
}

std::string ArrayList::display() const {
    std::stringstream ss;
    ss << "[";
    for (size_t i = 0; i < _data.size(); i++) {
        ss << _data[i];
        if (i < _data.size() - 1) ss << ", ";
    }
    ss << "]";
    return ss.str();
}