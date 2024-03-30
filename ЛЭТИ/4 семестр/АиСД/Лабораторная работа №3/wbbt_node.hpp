#pragma once
#include <compare>
#include <cstddef>

template <typename T> struct Node {
    T value;
    Node *left = nullptr;
    Node *right = nullptr;
    size_t _size;

    explicit Node(T value, Node<T> *left = nullptr, Node<T> *right = nullptr)
        : value(value), left(left), right(right), _size(1) {
        if (right != nullptr) {
            _size += right->_size;
        }
        if (left != nullptr) {
            _size += left->_size;
        }
    }

    size_t size() const {
        size_t result = 0;
        if (left != nullptr) {
            result += left->size();
        }
        if (right != nullptr) {
            result += right->size();
        }
        return result + 1;
    }
    size_t weight() const { return size() - 1; }
    void safe_delete() {
        left = nullptr;
        right = nullptr;
        delete this;
    }

    ~Node() {
        if (left != nullptr) {
            delete left;
        }
        if (right != nullptr) {
            delete right;
        }
    }

    Node<T> &operator=(const Node<T> &other) {
        this->value = other.value;
        return *this;
    }

    auto operator<=>(const Node<T> &other) const {
        return value <=> other.value;
    }

    bool operator==(const Node<T> &other) const = default;
    bool has_left() const { return left != nullptr; }
    bool has_right() const { return right != nullptr; }
};

static_assert(std::three_way_comparable<Node<int>>);
