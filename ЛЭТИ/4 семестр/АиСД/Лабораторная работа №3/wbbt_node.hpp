#pragma once
#include "consts.hpp"
#include <compare>
#include <cstddef>

template <typename T> struct Node {
    T value;
    Node *parent = nullptr;
    Node *left = nullptr;
    Node *right = nullptr;
    size_t _size;

    explicit Node(T value, Node<T> *left = nullptr, Node<T> *right = nullptr)
        : value(value), parent(nullptr), left(left), right(right), _size(1) {
        if (right != nullptr) {
            _size += right->_size;
            right->parent = this;
        }
        if (left != nullptr) {
            _size += left->_size;
            left->parent = this;
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
    void prepare_safe_delete() {
        left = nullptr;
        right = nullptr;
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
    bool is_left_child() const {
        return parent != nullptr && parent->left == this;
    }
    bool is_right_child() const {
        return parent != nullptr && parent->right == this;
    }
    bool has_left() const { return left != nullptr; }
    bool has_right() const { return right != nullptr; }
    bool has_parent() const { return parent != nullptr; }
};

static_assert(std::three_way_comparable<Node<int>>);