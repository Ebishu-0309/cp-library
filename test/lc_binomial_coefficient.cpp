// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/binomial_coefficient

#include "../template.hpp"

#include "../arbitrary_mod_binomial.hpp"

int main() {
    int t, m;
    cin >> t >> m;
    ArbitraryModBinomial solver(m);
    while (t--) {
        long long n, k;
        cin >> n >> k;
        cout << solver.calc(n, k) << "\n";
    }
}