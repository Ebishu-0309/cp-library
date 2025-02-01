// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/point_add_rectangle_sum

#include "../template.hpp"

#include "../wavelet_matrix_fenwick_tree.hpp"

int main() {
    int n, q;
    cin >> n >> q;
    vector<int> x(n), y(n);
    vector<lint> w(n);
    vector<pair<int, int>> ps;
    ps.reserve(n + q);
    rep(i, n) {
        cin >> x[i] >> y[i] >> w[i];
        ps.emplace_back(x[i], y[i]);
    }
    vector<array<int, 4>> qs(q);
    rep(i, q) {
        int t;
        cin >> t;
        if (t == 0) {
            cin >> qs[i][0] >> qs[i][1] >> qs[i][2];
            qs[i][3] = -1;
            ps.emplace_back(qs[i][0], qs[i][1]);
        } else {
            cin >> qs[i][0] >> qs[i][1] >> qs[i][2] >> qs[i][3];
        }
    }
    sort(all(ps));
    ps.erase(unique(all(ps)), ps.end());
    int n1 = ps.size();
    vector<int> y1(n1);
    vector<lint> w1(n1);
    rep(i, n1) y1[i] = ps[i].second;
    rep(i, n) { w1[lower_bound(all(ps), pair{x[i], y[i]}) - ps.begin()] += w[i]; }

    CompressedWaveletMatrixFenwickTree wm(y1, w1);
    rep(i, q) {
        if (qs[i][3] == -1) {
            auto [x, y, w, ign] = qs[i];
            wm.add(lower_bound(all(ps), pair{x, y}) - ps.begin(), w);
        } else {
            auto [l, d, r, u] = qs[i];
            cout << wm.range_sum(lower_bound(all(ps), pair{l, -IntInf}) - ps.begin(), lower_bound(all(ps), pair{r, -IntInf}) - ps.begin(), d, u)
                 << "\n";
        }
    }
}