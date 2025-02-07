// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/wildcard_pattern_matching

#include "../template.hpp"
#include "../wildcard_pattern_matching_deterministic.hpp"

int main() {
    string s, t;
    cin >> s >> t;
    for (bool e : wildcard_pattern_matching_deterministic(s, t)) cout << e;
    cout << endl;
}