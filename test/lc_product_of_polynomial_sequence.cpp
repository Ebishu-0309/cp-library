// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/product_of_polynomial_sequence

#include "../template.hpp"

#include "../formal_power_series.hpp"

int main() {
    int n;
    cin >> n;
    vector<FPS> fs(n);
    for (int i = 0; i < n; ++i) {
        int d;
        cin >> d;
        fs[i].resize(d + 1);
        cin >> fs[i];
    }
    auto prod = FPS::prod(fs);
    cout << prod << endl;
}