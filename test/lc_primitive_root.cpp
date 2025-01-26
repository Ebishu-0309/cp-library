// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/primitive_root

#include "../template.hpp"

#include "../number/primitive_root.hpp"

int main() {
    int q;
    cin >> q;
    while (q--) {
        long long p;
        cin >> p;
        cout << primitive_root(p) << endl;
    }
}