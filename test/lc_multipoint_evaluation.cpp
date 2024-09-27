// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/multipoint_evaluation

#include "../template.hpp"

#include "../formal_power_series.hpp"

int main() {
    int n, m;
    cin >> n >> m;

    FormalPowerSeries<Mod> f(n);
    for (mint& e : f) {
        int c;
        cin >> c;
        e = mint::raw(c);
    }

    vector<mint> p(m);
    for (mint& e : p) {
        int p1;
        cin >> p1;
        e = mint::raw(p1);
    }

    cout << f.eval(p) << endl;
}