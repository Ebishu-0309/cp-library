// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/binomial_coefficient

#include "../template.hpp"
#include "../wildcard_pattern_matching.hpp"

int main() {
    string s, t;
    cin >> s >> t;
    for (bool e : wildcard_pattern_matching(s, t)) cout << e;
    cout << endl;
}