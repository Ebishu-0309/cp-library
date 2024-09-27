// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/factorial

#include "../template.hpp"

#include "../formal_power_series.hpp"

int main() {
    int t;
    cin >> t;
    while (t--) {
        int n;
        cin >> n;
        cout << FPS::factorial(n) << endl;
    }
}