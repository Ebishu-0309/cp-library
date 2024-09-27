// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/area_of_union_of_rectangles

#include "../template.hpp"

#include "../rectangle_union.hpp"

int main() {
    int n;
    cin >> n;
    vector<int> l(n), r(n), d(n), u(n);
    for (int i = 0; i < n; ++i) {
        cin >> l[i] >> d[i] >> r[i] >> u[i];
    }
    cout << rectangle_union(l, r, d, u) << "\n";
}