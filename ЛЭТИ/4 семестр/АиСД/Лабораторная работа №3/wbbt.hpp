#pragma once
#include "comparable.hpp"
#include "consts.hpp"
#include "wbbt_iterator.hpp"
#include "wbbt_node.hpp"
#include <algorithm>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <ostream>
#include <stdexcept>
#include <tuple>
#include <vector>

// Weight balanced binary tree
template <Comparable T> class WBBT {
    using node = Node<T> *;

    node _root;

    explicit WBBT(WBBT<T> &&other) = delete; // delete move constructor

    node _insert(T value, node root) {
        if (root == nullptr) {
            return new Node(value);
        }
        node result;
        if (value < root->value) {
            result =
                balanceL(root->value, _insert(value, root->left), root->right);
            root->safe_delete();
        } else if (value > root->value) {
            result =
                balanceR(root->value, root->left, _insert(value, root->right));
            root->safe_delete();
        } else {
            result = root;
        }
        return result;
    }

    node balanceL(T value, node left, node right) {
        node result;
        if (right == nullptr) {
            if (left == nullptr) {
                return new Node(value);
            }
            if (left->size() == 1) {
                return new Node(value, left);
            }
            if (!left->has_left()) {
                result =
                    new Node(**left->right, new Node(**left), new Node(value));
            } else if (!left->has_right()) {
                result = new Node(**left, left->left, new Node(value));
            } else if (_size(left->right) < GAMMA * _size(left->left)) {
                result =
                    new Node(**left, left->left, new Node(value, left->right));
            } else {
                result =
                    new Node(**left->right,
                             new Node(**left, left->left, left->right->left),
                             new Node(value, left->right->right));
                left->right->safe_delete();
            }
            left->safe_delete();
            return result;
        }
        if (left == nullptr) {
            return new Node(value, node(nullptr), right);
        }
        if (_size(left) > DELTA * _size(right)) {
            if (_size(left->right) < GAMMA * _size(left->left)) {
                result = new Node(**left, left->left,
                                  new Node(value, left->right, right));
            } else {
                result =
                    new Node(**left->right,
                             new Node(**left, left->left, left->right->left),
                             new Node(value, left->right->right, right));
                left->right->safe_delete();
            }
            left->safe_delete();
            return result;
        } else {
            return new Node(value, left, right);
        }
    };

    node rotateL(T value, node left, node right) {
        if (isSingle(right->left, right->right)) {
            return singleL(value, left, right);
        } else {
            return doubleL(value, left, right);
        }
    }

    node singleL(T value, node left, node right) {
        auto result = new Node(right->value, new Node(value, left, right->left),
                               right->right);
        right->safe_delete();
        return result;
    }

    node doubleL(T value, node left, node right) {
        auto result = new Node(
            right->left->value, new Node(value, left, right->left->left),
            new Node(right->value, right->left->right, right->right));
        right->left->safe_delete();
        right->safe_delete();
        return result;
    }

    node balanceR(T value, node left, node right) {
        node result;
        if (left == nullptr) {
            if (right == nullptr) {
                return new Node(value);
            }
            if (right->size() == 1) {
                return new Node(value, (node) nullptr, right);
            }
            if (!right->has_left()) {
                result = new Node(right->value, new Node(value), right->right);
            } else if (!right->has_right()) {
                result = new Node(right->left->value, new Node(value),
                                  new Node(right->value));
            } else if (_size(right->left) < GAMMA * _size(right->right)) {
                result =
                    new Node(right->value, new Node(value, left, right->left),
                             right->right);
            } else {
                result = new Node(
                    right->left->value,
                    new Node(value, (node) nullptr, right->left->left),
                    new Node(right->value, right->left->right, right->right));
                right->left->safe_delete();
            }
            right->safe_delete();
            return result;
        }
        if (right == nullptr) {
            return new Node(value, left);
        }
        if (_size(right) > DELTA * _size(left)) {
            if (_size(right->left) < GAMMA * _size(right->right)) {
                result = new Node(**right, new Node(value, left, right->left),
                                  right->right);
            } else {
                result = new Node(
                    **right->left, new Node(value, left, right->left->left),
                    new Node(**right, right->left->right, right->right));
                right->left->safe_delete();
            }
            right->safe_delete();
            return result;
        } else {
            return new Node(value, left, right);
        }
    };

    node rotateR(T value, node left, node right) {
        if (isSingle(left->right, left->left)) {
            return singleR(value, left, right);
        } else {
            return doubleR(value, left, right);
        }
    }

    node singleR(T value, node left, node right) {
        auto result = new Node(left->value, new Node(value, right, left->right),
                               left->left);
        left->safe_delete();
        return result;
    }

    node doubleR(T value, node left, node right) {
        auto result = new Node(
            left->right->value, new Node(value, right, left->right->right),
            new Node(left->value, left->right->left, left->left));
        left->right->safe_delete();
        left->safe_delete();
        return result;
    }

    size_t _size(node x) const { return x == nullptr ? 0 : x->size(); }

    bool isBalanced(node a, node b) {
        return (DELTA * (_size(a) + 1)) >= (_size(b) + 1);
    }

    bool isSingle(node a, node b) {
        return (_size(a) + 1) < GAMMA * (_size(b) + 1);
    }

    Node<T> *_clone_subtree(const Node<T> *source) {
        if (source != nullptr) {
            auto dest = new Node(source->value, _clone_subtree(source->left),
                                 _clone_subtree(source->right));
            return dest;
        }
        return nullptr;
    }

    node _remove(T value, node root) {
        if (root == nullptr) {
            return root;
        }
        node result;
        if (value < root->value) {
            result =
                balanceR(root->value, _remove(value, root->left), root->right);
        } else if (value > root->value) {
            result =
                balanceL(root->value, root->left, _remove(value, root->right));
        } else {
            result = glue(root->left, root->right);
        }
        root->safe_delete();
        return result;
    }

    node glue(node left, node right) {
        if (left == nullptr) {
            return right;
        }
        if (right == nullptr) {
            return left;
        }
        if (_size(left) > _size(right)) {
            auto [val, new_left_node] = deleteFindMax(left);
            return balanceR(val, new_left_node, right);
        }
        auto [val, new_right_node] = deleteFindMin(right);
        return balanceL(val, left, new_right_node);
    }

    std::tuple<T, node> deleteFindMin(node root) {
        if (root == nullptr) {
            throw std::logic_error("deleteFindMin: can not return the minimal "
                                   "element of an empty set");
        }
        if (root->left == nullptr) {
            std::tuple<T, node> result = {root->value, root->right};
            root->safe_delete();
            return result;
        }
        auto [value, new_left_node] = deleteFindMin(root->left);
        std::tuple<T, node> result = {
            value, balanceR(root->value, new_left_node, root->right)};
        root->safe_delete();
        return result;
    }

    std::tuple<T, node> deleteFindMax(node root) {
        if (root == nullptr) {
            throw std::logic_error("deleteFindMax: can not return the maximal "
                                   "element of an empty set");
        }
        if (root->right == nullptr) {
            std::tuple<T, node> result = {root->value, root->left};
            root->safe_delete();
            return result;
        }
        auto [value, new_right_node] = deleteFindMax(root->right);
        std::tuple<T, node> result = {
            value, balanceL(root->value, root->left, new_right_node)};
        root->safe_delete();
        return result;
    }

  public:
    using const_iterator = WBBTConstIterator<T>;
    using iterator = WBBTConstIterator<T>;
    using self_type = WBBT<T>;
    using value_type = T;

    WBBT() : _root(nullptr) {}
    WBBT(const WBBT<T> &other) { this->_root = new Node(*other._root); }
    WBBT<T> &operator=(const WBBT<T> &other) {
        this->~WBBT<T>();
        _root = new Node(*other._root);
        return *this;
    }

    WBBT<T> operator-(const WBBT<T> &other) const {
        WBBT<T> result(*this);
        for (auto iter = other.begin(); iter != other.end(); iter++) {
            result.erase(**iter);
        }
        return std::move(result);
    }

    WBBT<T> operator+(const WBBT<T> &other) const {
        WBBT<T> result;
        std::merge(begin(), end(), other.begin(), other.end(),
                   std::inserter(result, result.begin()));
        return std::move(result);
    }

    WBBT<T> operator|(const WBBT<T> &other) const {
        return std::move(*this + other);
    }

    WBBT<T> operator&(const WBBT<T> &other) const {
        WBBT<T> result;
        for (auto iter = begin(); iter != end(); iter++) {
            if (other.contains(**iter)) {
                result.insert(**iter);
            }
        }
        return std::move(result);
    }

    WBBT<T> operator^(const WBBT<T> &other) const {
        WBBT<T> result;
        std::set_symmetric_difference(begin(), end(), other.begin(),
                                      other.end(),
                                      std::inserter(result, result.begin()));
        return std::move(result);
    }

    const_iterator cbegin() const { return const_iterator(_root); }
    const_iterator cend() const { return const_iterator(nullptr); }
    const_iterator begin() const { return const_iterator(_root); }
    const_iterator end() const { return const_iterator(nullptr); }
    iterator begin() { return iterator(_root); }
    iterator end() { return iterator(nullptr); }

    bool contains(T value) {
        for (auto ptr = _root; ptr != nullptr;) {
            if (value == ptr->value) {
                return true;
            }
            if (value > ptr->value) {
                ptr = ptr->right;
            } else if (value < ptr->value) {
                ptr = ptr->left;
            }
        }
        return false;
    }

    void insert(T value) { _root = _insert(value, _root); }
    void insert(const_iterator &begin, const const_iterator &end) {
        for (; begin != end; begin++) {
            _root = _insert(begin.ptr->value, _root);
        }
    }
    constexpr iterator &insert(iterator &iter, T &&value) {
        _root = _insert(std::move(value), _root);
        return iter;
    }
    void erase(T value) { _root = _remove(value, _root); }
    template <typename Iter> auto erase(const Iter &begin, const Iter &end) {
        for (Iter iter = begin; iter != end; iter++) {
            erase(*iter);
        }
    }

    size_t size() const { return _size(_root); }

    void print(std::ostream &out, const Node<T> &node,
               std::vector<const Node<T> *> &path) const;
    ~WBBT() {
        if (_root != nullptr) {
            delete _root;
        }
    }

    template <typename T2>
    friend std::ostream &operator<<(std::ostream &out, const WBBT<T2> &tree);
};

template <typename T>
std::ostream &operator<<(std::ostream &out, const WBBT<T> &tree) {
    std::vector<const Node<T> *> path;
    if (tree._root == nullptr) {
        out << "<empty WBBT>" << std::endl;
    } else {
        tree.print(out, *tree._root, path);
    }
    return out;
}

template <Comparable T>
void WBBT<T>::print(std::ostream &out, const Node<T> &node,
                    std::vector<const Node<T> *> &path) const {
    path.push_back(&node);
    for (size_t i = 1; i < path.size() - 1; i++) {
        if (path[i - 1]->has_right() && path[i - 1]->right != path[i]) {
            out << "│";
        } else {
            out << " ";
        }
        out << "   ";
    }
    if (path.size() > 1) {
        if ((*(path.end() - 2))->left == &node) {
            if (!path.empty() && (*(path.end() - 2))->has_right()) {
                out << "├";
            } else {
                out << "└";
            }
            out << "───l:";
        } else {
            out << "└───r:";
        }
    }
    out << node.value << std::endl;
    if (node.has_left()) {
        print(out, *node.left, path);
    }
    if (node.has_right()) {
        print(out, *node.right, path);
    }
    path.pop_back();
}