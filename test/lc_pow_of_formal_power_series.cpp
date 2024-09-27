// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/pow_of_formal_power_series

#include "../template.hpp"

#include "../formal_power_series.hpp"

int main() {
    int n;
    long long m;
    cin >> n >> m;

    FormalPowerSeries<Mod> f(n);
    for (mint& e : f) {
        int a;
        cin >> a;
        e = mint::raw(a);
    }

    auto ans = f.pow(m);

    for (const mint& e : ans) cout << e.val() << " ";
}