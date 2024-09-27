// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/inv_of_formal_power_series

#include "../template.hpp"

#include "../formal_power_series.hpp"

int main() {
    int n;
    cin >> n;
    FormalPowerSeries<Mod> f(n);
    for (mint& e : f) {
        int a;
        cin >> a;
        e = mint::raw(a);
    }

    auto ans = f.inv();

    for (const mint& e : ans) cout << e.val() << " ";
}