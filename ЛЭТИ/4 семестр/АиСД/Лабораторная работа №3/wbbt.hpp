#pragma once
#include "consts.hpp"
#include "wbbt_iterator.hpp"
#include "wbbt_node.hpp"
#include <cstddef>
#include <ostream>
#include <vector>

// Weight balanced binary tree
template <typename T> class WBBT {
    using const_iterator = WBBTConstIterator<T>;
    using iterator = WBBTOutIterator<T>;
    using node = Node<T> *;

    node _root;

    explicit WBBT(WBBT<T> &&other) = delete; // delete move constructor

    node _insert(T value, node root) {
        if (root == nullptr) {
            return new Node(value);
        }
        if (value < root->value) {
            return balanceR(root->value, _insert(value, root->left),
                            root->right);
        }
        if (value > root->value) {
            return balanceL(root->value, root->left,
                            _insert(value, root->right));
        }
        return root;
    }

    node balanceL(T value, node left, node right) {
        if (isBalanced(left, right)) {
            return new Node(value, left, right);
        }
        return rotateL(value, left, right);
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
        right->prepare_safe_delete();
        delete right;
        return result;
    }

    node doubleL(T value, node left, node right) {
        auto result = new Node(
            right->left->value, new Node(value, left, right->left->left),
            new Node(right->value, right->left->right, right->right));
        right->left->prepare_safe_delete();
        delete right->left;
        return result;
    }

    node balanceR(T value, node left, node right) {
        if (isBalanced(left, right)) {
            return new Node(value, left, right);
        }
        return rotateR(value, left, right);
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
        left->prepare_safe_delete();
        delete left;
        return result;
    }

    node doubleR(T value, node left, node right) {
        auto result = new Node(
            left->right->value, new Node(value, right, left->right->right),
            new Node(left->value, left->right->left, left->left));
        left->right->prepare_safe_delete();
        delete left->right;
        return result;
    }

    bool isBalanced(node a, node b) {
        auto a_size = a == nullptr ? 0 : a->size();
        auto b_size = b == nullptr ? 0 : b->size();
        return (delta * (a_size + 1)) >= (b_size + 1);
    }

    bool isSingle(node a, node b) {
        auto a_size = a == nullptr ? 0 : a->size();
        auto b_size = b == nullptr ? 0 : b->size();
        return (a_size + 1) < gamma * (b_size + 1);
    }

    Node<T> *_clone_subtree(const Node<T> *source) {
        if (source != nullptr) {
            auto dest = new Node(source->value, _clone_subtree(source->left),
                                 _clone_subtree(source->right));
            return dest;
        }
        return nullptr;
    }

  public:
    WBBT() : _root(nullptr) {}
    WBBT(const WBBT<T> &other) = default;
    WBBT<T> &operator=(const WBBT<T> &other) {
        this->~WBBT<T>();
        _root = _clone_subtree(other._root);
        return *this;
    }

    const_iterator begin() const { return const_iterator(_root); }
    const_iterator end() const { return const_iterator(nullptr); }
    iterator begin() { return iterator(this, _root); }
    iterator end() { return iterator(this, nullptr); }

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

    size_t size() const { return _root == nullptr ? 0 : _root->size(); }

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
    tree.print(out, *tree._root, path);
    return out;
}

template <typename T>
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
