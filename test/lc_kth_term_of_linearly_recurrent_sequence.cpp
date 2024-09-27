// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/kth_term_of_linearly_recurrent_sequence

#include "../template.hpp"

#include "../formal_power_series.hpp"

int main() {
    long long d, k;
    cin >> d >> k;
    vector<mint> a(d), c(d);
    cin >> a >> c;
    cout << FPS::kth_term(a, c, k) << endl;
}