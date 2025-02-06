// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/static_range_mode_query

#include "../template.hpp"

#include "../static_range_mode_query.hpp"

int main() {
    int n, q;
    cin >> n >> q;

    vector<int> a(n);
    for (int& e : a) cin >> e;

    StaticRangeModeQuery solver(a);

    while (q--) {
        int l, r;
        cin >> l >> r;
        auto [mode, cnt] = solver.calc(l, r);
        cout << mode << " " << cnt << "\n";
    }
}