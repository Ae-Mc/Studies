#pragma once
#include <algorithm>
#include <compare>
#include <cstddef>

template <typename T> struct Node {
    T value;
    Node *left = nullptr;
    Node *right = nullptr;

    explicit Node(T value, Node<T> *left = nullptr, Node<T> *right = nullptr)
        : value(value), left(left), right(right), _size(1) {
        if (right != nullptr) {
            _size += right->_size;
        }
        if (left != nullptr) {
            _size += left->_size;
        }
    }
    Node(const Node &other) : value(other.value), _size(1) {
        if (other.has_left()) {
            left = new Node(*other.left);
            _size += left->size();
        }
        if (other.has_right()) {
            right = new Node(*other.right);
            _size += right->size();
        }
    }
    Node(Node &&other)
        : value(std::move(other.value)), left(std::move(other.left)),
          right(std::move(other.right)), _size(std::move(other._size)) {}

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

    auto operator<=>(const Node<T> &other) const {
        return value <=> other.value;
    }

    bool operator==(const Node<T> &other) const = default;
    bool has_left() const { return left != nullptr; }
    bool has_right() const { return right != nullptr; }

    Node<T> &operator=(Node<T> &other) {
        this->~Node();
        left = nullptr;
        right = nullptr;
        value = other.value;
        _size = 1;
        if (other.has_left()) {
            *left = new Node(*other.left);
            _size += left->size();
        }
        if (other.has_right()) {
            *right = new Node(*other.right);
            _size += right->size();
        }
    }

    Node<T> &operator=(Node<T> &&other) {
        value = std::move(other.value);
        if (left) {
            delete left;
        }
        left = std::move(other.left);
        if (right) {
            delete right;
        }
        right = std::move(other.right);
        _size = std::move(other._size);
    }

    operator auto() const { return value; }

    T &operator*() { return value; }
    const T &operator*() const { return value; }

  protected:
    size_t _size;
};

static_assert(std::three_way_comparable<Node<int>>);
