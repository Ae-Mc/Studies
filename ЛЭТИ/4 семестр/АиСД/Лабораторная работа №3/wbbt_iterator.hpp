#pragma once
// #include "wbbt.hpp"
#include "wbbt_node.hpp"
#include <cstddef>
#include <iterator>

template <typename T> class WBBT;

template <typename T> struct WBBTConstIterator {
    using self_type = WBBTConstIterator<T>;
    using value_type = const Node<T>;
    using reference = const Node<T> &;
    using pointer = const Node<T> *;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::forward_iterator_tag;

    WBBTConstIterator(pointer ptr = nullptr, self_type *root = nullptr)
        : ptr(ptr), root(root){};

    // prefix increment
    self_type &operator++() {
        if (ptr != nullptr) {
            if (ptr->has_right()) {
                root = new self_type(ptr, root);
            }
            if (ptr->has_left()) {
                ptr = ptr->left;
            } else {
                if (root == nullptr) {
                    ptr = nullptr;
                } else {
                    auto temp = root;
                    ptr = root->ptr->right;
                    root = root->root;
                    delete temp;
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

  protected:
    pointer ptr;
    self_type *root;
};

static_assert(std::forward_iterator<WBBTConstIterator<int>>);

template <typename T> struct WBBTOutIterator : public WBBTConstIterator<T> {
    using self_type = WBBTOutIterator;
    using value_type = Node<T>;
    using reference = Node<T> &;
    using pointer = Node<T> *;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::output_iterator_tag;

    WBBTOutIterator(WBBT<T> *container, pointer ptr = nullptr,
                    self_type *root = nullptr)
        : WBBTConstIterator<T>(ptr, root), container(container){};

    self_type &operator++() {
        WBBTConstIterator<T>::operator++();
        return *this;
    }

    self_type operator++(int) {
        self_type &tmp = *this;
        ++(*this);
        return tmp;
    }

    self_type &operator*() { return *this; }
    pointer operator->() { return WBBTConstIterator<T>::ptr; }
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

  protected:
    WBBT<T> *container;
};

static_assert(std::indirectly_writable<WBBTOutIterator<int>, Node<int>>);
static_assert(std::output_iterator<WBBTOutIterator<int>, Node<int>>);