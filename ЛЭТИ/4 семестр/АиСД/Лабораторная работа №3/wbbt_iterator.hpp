#pragma once
// #include "wbbt.hpp"
#include "wbbt_node.hpp"
#include <cstddef>
#include <iterator>

template <typename T> class WBBT;

template <typename T> struct WBBTConstIterator {
    using self_type = WBBTConstIterator;
    using value_type = const Node<T>;
    using reference = const Node<T> &;
    using pointer = const Node<T> *;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::forward_iterator_tag;

    WBBTConstIterator(pointer ptr = nullptr) : ptr(ptr){};

    // prefix increment
    self_type &operator++() {
        if (ptr != nullptr) {
            if (ptr->left != nullptr) {
                ptr = ptr->left;
            } else if (ptr->right != nullptr) {
                ptr = ptr->right;
            } else {
                bool prev_is_right;
                do {
                    prev_is_right = ptr->is_right_child();
                    ptr = ptr->parent;
                } while (ptr != nullptr &&
                         (prev_is_right || ptr->right == nullptr));
                if (ptr != nullptr) {
                    ptr = ptr->right;
                }
            }
        }
        return *this;
    }

    // postfix increment
    self_type operator++(int) {
        self_type &tmp = *this;
        ++(*this);
        return tmp;
    }

    reference operator*() const { return *ptr; }
    pointer operator->() { return ptr; }
    friend bool operator==(const WBBTConstIterator &a,
                           const WBBTConstIterator &b) {
        return a.ptr == b.ptr;
    };
    friend bool operator!=(const WBBTConstIterator &a,
                           const WBBTConstIterator &b) {
        return a.ptr != b.ptr;
    };

  private:
    pointer ptr;
};

static_assert(std::forward_iterator<WBBTConstIterator<int>>);

template <typename T> struct WBBTOutIterator {
    using self_type = WBBTOutIterator;
    using value_type = Node<T>;
    using reference = Node<T> &;
    using pointer = Node<T> *;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::output_iterator_tag;

    WBBTOutIterator(WBBT<T> *container, pointer ptr = nullptr)
        : ptr(ptr), container(container){};

    // prefix increment
    self_type &operator++() {
        if (ptr != nullptr) {
            if (ptr->left != nullptr) {
                ptr = ptr->left;
            } else if (ptr->right != nullptr) {
                ptr = ptr->right;
            } else {
                bool prev_is_right;
                do {
                    prev_is_right = ptr->is_right_child();
                    ptr = ptr->parent;
                } while (ptr != nullptr &&
                         (prev_is_right || ptr->right == nullptr));
                if (ptr != nullptr) {
                    ptr = ptr->right;
                }
            }
        }
        return *this;
    }

    // postfix increment
    self_type operator++(int) {
        self_type &tmp = *this;
        ++(*this);
        return tmp;
    }

    self_type &operator*() { return *this; }
    pointer operator->() { return ptr; }
    self_type operator=(const value_type &node) {
        this->container->insert(node.value);
        return *this;
    }
    friend bool operator==(const WBBTOutIterator &a, const WBBTOutIterator &b) {
        return a.ptr == b.ptr;
    };
    friend bool operator!=(const WBBTOutIterator &a, const WBBTOutIterator &b) {
        return a.ptr != b.ptr;
    };

  private:
    pointer ptr;
    WBBT<T> *container;
};

static_assert(std::indirectly_writable<WBBTOutIterator<int>, Node<int>>);
static_assert(std::output_iterator<WBBTOutIterator<int>, Node<int>>);