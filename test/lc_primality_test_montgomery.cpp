// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/primality_test

#include "../template.hpp"

#include "../number/miller_rabin_montgomery.hpp"

int main() {
    int q;
    cin >> q;
    while (q--) {
        lint n;
        cin >> n;
        YesNo(is_prime_montgomery(n));
    }
}