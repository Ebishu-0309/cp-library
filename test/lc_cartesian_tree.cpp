// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/cartesian_tree

#include "../template.hpp"

#include "../cartesian_tree.hpp"

int main() {
    int n;
    cin >> n;
    vector<int> a(n);
    cin >> a;
    auto p = cartesian_tree(a);
    rep(i, n) if (p[i] == -1) p[i] = i;
    cout << p << endl;
}