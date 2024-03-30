#include "wbbt.hpp"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <set>

/*
Task (5):
A ⊕ B \ (C ∪ D) \ E
WBBT with MERGE, CONCAT, SUBST
*/

using namespace std;

template <typename T> void depth_search(const WBBT<T> &tree) {
    for (auto iter = tree.begin(); iter != tree.end(); ++iter) {
        if (iter != tree.begin()) {
            cout << "->";
        }
        cout << iter->value;
    }
    cout << endl;
}

int main() {
    WBBT<int> A, B, result;
    for (int i = 500; i < 1000; i++) {
        A.insert(i);
    }
    for (int i = 0; i < 500; i++) {
        A.insert(i);
    }
    for (int i = 64; i < 128; i++) {
        B.insert(i);
    }
    cout << "Tree size: " << A.size() << endl;

    cout << A;
    depth_search(A);
    for (const auto i : {1, 2, 3, 148, 53, 64, -1024, -1, 0, 23, 62}) {
        cout << "Tree contains " << i << " is "
             << (A.contains(i) ? "true" : "false") << endl;
    }

    merge(cbegin(B), cend(B), cbegin(A), cend(A), begin(result));
    // cout << "Tree merge result:" << endl;
    // cout << result;
    // depth_search(result);
    return 0;
}