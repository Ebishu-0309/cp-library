// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/discrete_logarithm_mod

#include "../template.hpp"

#include "../number/modlog.hpp"

int main() {
    int t;
    cin >> t;
    while (t--) {
        int x, y, m;
        cin >> x >> y >> m;
        cout << modlog(x, y, m) << endl;
    }
}