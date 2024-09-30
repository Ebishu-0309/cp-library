// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/unionfind_with_potential

#include "../template.hpp"

#include "../potential_union_find.hpp"

int main() {
    int n, q;
    cin >> n >> q;
    PotentialUnionFind<mint> uf(n);
    while (q--) {
        int t;
        cin >> t;
        if (t == 0) {
            int u, v;
            mint x;
            cin >> u >> v >> x;
            cout << uf.unite(u, v, x) << "\n";
        } else {
            int u, v;
            cin >> u >> v;
            auto opt = uf.delta(u, v);
            if (opt.has_value())
                cout << opt.value() << "\n";
            else
                cout << "-1\n";
        }
    }
}