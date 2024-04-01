#include "wbbt.hpp"
#include "wbbt_iterator.hpp"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <ranges>
#include <regex>
#include <set>
#include <sstream>
#include <string>
#include <vector>

/*
Task (5):
A ⊕ B \ (C ∪ D) \ E
WBBT with MERGE, CONCAT, SUBST
*/

constinit int tree_size = 200;

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

ostream &operator<<(ostream &out, std::ranges::range auto list) {
    out << "[";
    for (auto iter = list.begin(); iter != list.end(); ++iter) {
        if (iter != list.begin()) {
            out << ',';
        }
        out << *iter;
    }
    out << "]";
    return out;
}

template <typename T>
istream &operator>>(istream &in, std::ranges::output_range<T> auto range) {
    string line;
    getline(in, line);
    istringstream line_stream(line);
    insert_iterator<T> iter = begin(range);
    while (!line_stream.eof()) {
        line_stream >> iter;
    }
    return in;
}

int main() {
    WBBT<int> A, B, result;
    cin >> A;
    cout << "Tree size: " << A.size() << endl;

    cout << A;
    depth_search(A);
    A.erase(0);
    // cout << "After deleteion:" << endl;
    // cout << "Tree size: " << A.size() << endl;
    // cout << A;
    for (const auto i :
         {1, 2, 3, 148, 53, 64, -1024, -1, 0, 23, 62, 54, 52, 51, 63}) {
        cout << "Tree contains " << i << " is "
             << (A.contains(i) ? "true" : "false") << endl;
        A.contains(i);
    }

    set<int> set_A(cbegin(A), cend(A)), set_B(cbegin(B), cend(B)), set_result;

    merge(begin(set_A), end(set_A), begin(set_B), end(set_B),
          std::inserter(set_result, set_result.begin()));
    merge(cbegin(A), cend(A), cbegin(B), cend(B),
          inserter(result, begin(result)));
    cout << "Tree merge size: " << result.size() << ". Result:" << endl;
    cout << result;
    auto set_predicate = [](const int &a) { return 20 < a && a < 100; };
    erase_if(set_result, set_predicate);
    result.erase(begin(set_result), end(set_result));
    cout << "After deletion of values outside of range (20, 100): Tree size "
         << result.size() << ". Tree:" << endl;
    cout << result << endl;
    return 0;
}
