// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/static_range_frequency

#include "../template.hpp"

#include "../wavelet_matrix.hpp"

int main() {
    int n, q;
    cin >> n >> q;
    vector<int> a(n);
    cin >> a;
    if (n != 0) {
        CompressedWaveletMatrix wm(a);
        while (q--) {
            int l, r, x;
            cin >> l >> r >> x;
            cout << wm.count(x, l, r) << "\n";
        }
    } else {
        while (q--) {
            int l, r, x;
            cin >> l >> r >> x;
            cout << 0 << "\n";
        }
    }
}