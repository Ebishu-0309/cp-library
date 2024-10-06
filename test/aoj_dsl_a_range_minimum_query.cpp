// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/problems/DSL_2_A

#include "../template.hpp"

#include "../dynamic_segtree.hpp"

using S = int;
S op(S a, S b) { return min(a, b); }
S e() { return 2147483647; }
int main() {
    int n, q;
    cin >> n >> q;
    DynamicSegtree<S, op, e> seg(0, n);

    while (q--) {
        int c;
        cin >> c;
        if (c == 0) {
            int i, x;
            cin >> i >> x;
            seg.set(i, x);
        } else {
            int l, r;
            cin >> l >> r;
            cout << seg.prod(l, r + 1) << "\n";
        }
    }
}