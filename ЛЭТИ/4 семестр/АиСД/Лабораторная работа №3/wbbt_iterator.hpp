#pragma once
#include "comparable.hpp"
#include "wbbt_node.hpp"
#include <cstddef>
#include <iterator>
#include <memory>
#include <ranges>
#include <type_traits>

template <Comparable T> class WBBT;

template <Comparable T> struct WBBTConstIterator {
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

template <Comparable T> struct WBBTInsertIterator {
    using iterator_category = std::output_iterator_tag;
    using value_type = void;
    using difference_type = std::ptrdiff_t;
    using pointer = void;
    using reference = void;
    using container_type = WBBT<T>;

    constexpr WBBTInsertIterator(container_type &container,
                                 std::ranges::iterator_t<container_type> i)
        : container(std::addressof(container)), iter(i){};

    constexpr std::insert_iterator<container_type> &
    operator=(const typename container_type::value_type &node) {
        this->container->insert(node.value);
        return *this;
    }

    constexpr std::insert_iterator<container_type> &
    operator=(const typename container_type::value_type &&node) {
        this->container->insert(node.value);
        return *this;
    }

    constexpr std::insert_iterator<container_type> &operator*() {
        return *this;
    }

    constexpr std::insert_iterator<container_type> &operator++() {
        return *this;
    }

    constexpr std::insert_iterator<container_type> &operator++(int) {
        return *this;
    }

  protected:
    container_type *container;
    std::ranges::iterator_t<container_type> iter;
};