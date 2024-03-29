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
    vector<int> sA, sB, sResult(200);
    for (int i = 0; i < 63; i++) {
        A.insert(i);
        sA.push_back(i);
    }
    for (int i = 64; i < 128; i++) {
        B.insert(i);
        sB.push_back(i);
    }
    set_union(begin(sA), end(sA), begin(sB), end(sB), sResult.begin());
    cout << "Tree size: " << result.size() << endl;
    for (auto element : sResult) {
        cout << element << " ";
    }
    cout << endl;

    cout << A;
    depth_search(A);
    for (const auto i :
         vector<int>({1, 2, 3, 148, 53, 64, -1024, -1, 0, 23, 62})) {
        cout << "Tree contains " << i << " is "
             << (A.contains(i) ? "true" : "false") << endl;
    }

    merge(cbegin(B), cend(B), cbegin(A), cend(A), begin(result));
    cout << "Tree merge result:" << endl;
    cout << result;
    depth_search(result);
    // WBBT<int> manual_merged;
    // for (auto elem : A) {
    //     manual_merged.insert(elem->value);
    // }
    // for (auto elem : B) {
    //     manual_merged.insert(elem->value);
    // }
    // cout << "Manual merged tree:" << endl;
    // cout << manual_merged;
    return 0;
}