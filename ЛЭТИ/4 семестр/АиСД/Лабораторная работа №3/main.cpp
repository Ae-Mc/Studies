#include "wbbt.hpp"
#include "wbbt_iterator.hpp"
#include <iostream>
#include <sstream>
#include <string>

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

template <template <class> class U, class T>
concept Insertable =
    requires(U<T> container, T value) { container.insert(value); };

template <template <class> class U, class T>
    requires Insertable<U, T>
istream &operator>>(istream &in, U<T> &container) {
    string line;
    getline(in, line);
    istringstream line_stream(line);
    while (!line_stream.eof()) {
        T x;
        line_stream >> x;
        container.insert(x);
    }
    return in;
}

int main() {
    WBBT<int> A, B, C, D, E, result;
    cout << "Enter A, B, C, D, E (values separated by space, sets separated by "
            "new lines):"
         << endl;
    cout << "A: ";
    cin >> A;
    cout << "B: ";
    cin >> B;
    cout << "C: ";
    cin >> C;
    cout << "D: ";
    cin >> D;
    cout << "E: ";
    cin >> E;

    result = (A ^ B) - (C | D) - E;
    cout << "A ⊕ B \\ (C ∪ D) \\ E:" << endl << result;
    return 0;
}
