// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/factorize

#include "../template.hpp"

#include "../number/pollard_rho.hpp"

int main() {
    int q;
    cin >> q;
    while (q--) {
        long long n;
        cin >> n;
        vector<long long> ps = factorize(n);
        cout << ps.size() << " " << ps << endl;
    }
}