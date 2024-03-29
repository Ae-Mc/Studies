#pragma once
#include "wbbt_iterator.hpp"
#include "wbbt_node.hpp"
#include <cstddef>
#include <ostream>
#include <utility>
#include <vector>

// Weight balanced binary tree
template <typename T> class WBBT {
    using const_iterator = WBBTConstIterator<T>;
    using iterator = WBBTOutIterator<T>;

    Node<T> *_root;

    explicit WBBT(WBBT<T> &&other) = delete; // delete move constructor

    bool _insert(T value, Node<T> *root) {
        if (root == nullptr) {
            return false;
        }
        if (root->value == value) {
            return true;
        }
        if (root->value < value) {
            if (!_insert(value, root->right)) {
                root->right = new Node(value);
                root->right->parent = root;
                _balance(root->right);
            }
        } else if (!_insert(value, root->left)) {
            root->left = new Node(value);
            root->left->parent = root;
            _balance(root->left);
        }
        return true;
    }

    void _balance(Node<T> *z) {
        // TODO: balancing working not properly
        typedef Node<T> *(RotationFunc)(Node<T> *, Node<T> *);
        Node<T> *g, *n, *x = z->parent;
        RotationFunc *double_rotation_func, *single_rotation_func;
        bool (*first_comparator)(int value), (*second_comparator)(int value);
        for (; x != nullptr; x = z->parent) {
            first_comparator = [](int value) { return value > 0; };
            second_comparator = [](int value) { return value < 0; };
            if (z->is_right_child()) {
                double_rotation_func = rotate_rightLeft;
                single_rotation_func = rotate_left;
            } else {
                double_rotation_func = rotate_leftRight;
                single_rotation_func = rotate_right;
                std::swap(first_comparator, second_comparator);
            }
            if (first_comparator(x->balance_factor())) {
                g = x->parent;
                if (second_comparator(z->balance_factor())) {
                    n = double_rotation_func(x, z);
                } else {
                    n = single_rotation_func(x, z);
                }
            } else {
                if (second_comparator(x->balance_factor())) {
                    break;
                }
                z = x;
                continue;
            }
            n->parent = g;
            if (g != nullptr) {
                if (x == g->left) {
                    g->left = n;
                } else {
                    g->right = n;
                }
            } else {
                this->_root = n;
            }
            break;
        }
    }

    static Node<T> *rotate_left(Node<T> *x, Node<T> *z) {
        Node<T> *t23;
        t23 = z->left;
        x->right = t23;
        if (t23 != nullptr) {
            t23->parent = x;
        }
        z->left = x;
        x->parent = z;
        return z;
    }

    static Node<T> *rotate_right(Node<T> *x, Node<T> *z) {
        Node<T> *t23;
        t23 = z->right;
        x->left = t23;
        if (t23 != nullptr) {
            t23->parent = x;
        }
        z->right = x;
        x->parent = z;
        return z;
    }

    static Node<T> *rotate_leftRight(Node<T> *x, Node<T> *z) {
        z = rotate_left(x, z);
        return rotate_right(x, z);
        // auto y = z->right;
        // auto t3 = y->left;
        // z->right = t3;
        // if (t3 != nullptr) {
        //     t3->parent = z;
        // }
        // y->left = z;
        // z->parent = y;
        // auto t2 = y->right;
        // x->left = t2;
        // if (t2 != nullptr) {
        //     t2->parent = x;
        // }
        // y->right = x;
        // x->parent = y;
        // return y;
    }

    static Node<T> *rotate_rightLeft(Node<T> *x, Node<T> *z) {
        z = rotate_right(x, z);
        return rotate_left(x, z);
    }

    Node<T> *_clone_subtree(const Node<T> *source) {
        if (source != nullptr) {
            auto dest = new Node(source->value);
            dest->right = _clone_subtree(source->right);
            if (dest->right != nullptr) {
                dest->right->parent = dest;
            }
            dest->left = _clone_subtree(source->left);
            if (dest->left != nullptr) {
                dest->left->parent = dest;
            }
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

    void insert(T value) {
        if (!_insert(value, _root)) {
            _root = new Node(value);
        }
        // if (!_root->balanced()) {
        //     if (_root->has_right()) {
        //         _balance(_root->right);
        //     }
        // }
    }

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
        if (path[i - 1]->has_right() && !path[i]->is_right_child()) {
            out << "│";
        } else {
            out << " ";
        }
        out << "   ";
    }
    if (node.is_left_child()) {
        if (node.has_parent() && node.parent->has_right()) {
            out << "├";
        } else {
            out << "└";
        }
        out << "───l:";
    } else if (node.is_right_child()) {
        out << "└───r:";
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
