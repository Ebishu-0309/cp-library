// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/problems/DSL_2_B

#include "../template.hpp"

#include "../dynamic_segtree.hpp"

using S = int;
S op(S a, S b) { return a + b; }
S e() { return 0; }
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
            i--;
            seg.set(i, seg.get(i) + x);
        } else {
            int l, r;
            cin >> l >> r;
            cout << seg.prod(l - 1, r) << "\n";
        }
    }
}