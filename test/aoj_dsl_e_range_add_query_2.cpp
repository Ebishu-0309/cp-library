// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/problems/DSL_2_E

#include "../template.hpp"

#include "../dual_segment_tree.hpp"

using F = int;
F composition(F f, F g) { return f + g; }
F id() { return 0; }
int main() {
    int n, q;
    cin >> n >> q;
    DualSegmentTree<F, composition, id> seg(n);
    while (q--) {
        int c;
        cin >> c;
        if (c == 0) {
            int l, r, x;
            cin >> l >> r >> x;
            l--;
            seg.apply(l, r, x);
        } else {
            int p;
            cin >> p;
            p--;
            cout << seg.get(p) << "\n";
        }
    }
}