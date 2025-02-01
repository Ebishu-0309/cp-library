// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/enumerate_palindromes

#include "../template.hpp"

#include "../manacher.hpp"

int main() {
    string s;
    cin >> s;
    int n = s.size();
    string t;
    t.reserve(n * 2 + 1);
    for (char c : s) {
        t += '$';
        t += c;
    }
    t += '$';
    auto r = manacher(t);
    vector<int> ans(2 * n - 1);
    for (int i = 0; i < 2 * n - 1; ++i) {
        ans[i] = r[i + 1] - 1;
    }
    cout << ans << endl;
}