// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/point_set_range_composite

#include "../template.hpp"

#include "../dynamic_segtree.hpp"
#include "../linear_function.hpp"

using S = LinearFunction<mint>;
S op(S f, S g) {  // g(f(x))
    return g(f);
}
int main() {
    int n, q;
    cin >> n >> q;
    DynamicSegtree<S, op, S::id> seg(0, n);
    rep(i, n) {
        S f;
        cin >> f;
        seg.set(i, f);
    }
    while (q--) {
        int t;
        cin >> t;
        if (t == 0) {
            int p;
            S f;
            cin >> p >> f;
            seg.set(p, f);
        } else {
            int l, r, x;
            cin >> l >> r >> x;
            cout << seg.prod(l, r).calc(x) << "\n";
        }
    }
}