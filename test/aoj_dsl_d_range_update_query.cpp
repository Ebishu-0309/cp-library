// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/problems/DSL_2_D

#include "../template.hpp"

#include "../dual_segment_tree.hpp"

using F = int;
F id() { return 2147483647; }
F composition(F f, F g) { return (f == id() ? g : f); }
int main() {
    int n, q;
    cin >> n >> q;
    DualSegmentTree<F, composition, id> seg(n);

    while (q--) {
        int c;
        cin >> c;
        if (c == 0) {
            int s, t, x;
            cin >> s >> t >> x;
            seg.apply(s, t + 1, x);
        } else {
            int p;
            cin >> p;
            cout << seg.get(p) << "\n";
        }
    }
}