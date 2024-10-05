// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/range_affine_point_get

#include "../template.hpp"

#include "../dual_segment_tree.hpp"
#include "../linear_function.hpp"

using F = LinearFunction<mint>;
F composition(F f, F g) { return f.composition(g); }
int main() {
    int n, q;
    cin >> n >> q;
    DualSegmentTree<F, composition, F::id> seg(n);
    vector<mint> a(n);
    cin >> a;
    while (q--) {
        int c;
        cin >> c;
        if (c == 0) {
            int l, r;
            F f;
            cin >> l >> r >> f;
            seg.apply(l, r, f);
        } else {
            int p;
            cin >> p;
            cout << seg.get(p).calc(a[p]) << "\n";
        }
    }
}