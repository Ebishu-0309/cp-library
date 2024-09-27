// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/range_kth_smallest

#include "../template.hpp"

#include "../wavelet_matrix.hpp"

int main() {
    int n, q;
    cin >> n >> q;
    vector<int> a(n);
    for (int& e : a) cin >> e;

    CompressedWaveletMatrix wm(a);

    while (q--) {
        int l, r, k;
        cin >> l >> r >> k;
        cout << wm.kth_smallest(l, r, k) << "\n";
    }
}