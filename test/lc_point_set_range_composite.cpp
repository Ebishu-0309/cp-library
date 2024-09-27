// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/point_set_range_composite

#include "../template.hpp"

#include "../dynamic_segtree.hpp"

struct S {
    mint a, b;
};
S op(S f, S g) {  // g(f(x))
    return S{g.a * f.a, g.a * f.b + g.b};
}
S e() { return S{1, 0}; }
int main() {
    int n, q;
    cin >> n >> q;
    DynamicSegtree<S, op, e> seg(0, n);
    rep(i, n) {
        int a, b;
        cin >> a >> b;
        seg.set(i, {mint::raw(a), mint::raw(b)});
    }
    while (q--) {
        int t;
        cin >> t;
        if (t == 0) {
            int p, c, d;
            cin >> p >> c >> d;
            seg.set(p, S{mint::raw(c), mint::raw(d)});
        } else {
            int l, r, x;
            cin >> l >> r >> x;
            S f = seg.prod(l, r);
            cout << (f.a * x + f.b).val() << "\n";
        }
    }
}