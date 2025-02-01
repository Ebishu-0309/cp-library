// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/rectangle_sum

#include "../template.hpp"

#include "../wavelet_matrix_sum.hpp"

int main() {
    int n, q;
    cin >> n >> q;
    vector<array<int, 3>> v(n);
    rep(i, n) { cin >> v[i][0] >> v[i][1] >> v[i][2]; }
    sort(all(v));
    vector<int> x(n), y(n);
    vector<lint> w(n);
    rep(i, n) {
        x[i] = v[i][0];
        y[i] = v[i][1];
        w[i] = v[i][2];
    }
    CompressedWaveletMatrixSum wm(y, w);
    while (q--) {
        int l, r, d, u;
        cin >> l >> d >> r >> u;
        l = lower_bound(all(x), l) - x.begin();
        r = lower_bound(all(x), r) - x.begin();
        cout << wm.range_sum(l, r, d, u) << "\n";
    }
}