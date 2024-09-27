// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/staticrmq

#include "../template.hpp"

#include "../sparse_table.hpp"

int op(int a, int b) { return min(a, b); }

int main() {
    int n, q;
    cin >> n >> q;
    vector<int> a(n);
    for (auto& e : a) cin >> e;

    SparseTable<int, op> sp(a);
    while (q--) {
        int l, r;
        cin >> l >> r;
        cout << sp.prod(l, r) << "\n";
    }
}