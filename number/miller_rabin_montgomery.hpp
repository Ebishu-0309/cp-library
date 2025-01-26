#include "montgomery.hpp"

bool miller_rabin_montgomery(long long n, const vector<long long> &ts) {
    const Montgomery mr(n);

    const long long s2 = (n - 1) & (1 - n), d = (n - 1) / s2;
    const auto pow_mod_n = [=](long long b, long long p) {
        long long res = 1;
        while (p > 0) {
            if (p & 1) res = mr.mul(res, b);
            b = mr.mul(b, b);
            p >>= 1;
        }
        return res;
    };
    for (const long long a : ts) {
        if (a >= n) break;
        auto ad = pow_mod_n(a, d);
        if (ad == 1) continue;
        long long t = 1;
        for (; t < s2; t <<= 1) {
            if (ad == n - 1) break;
            ad = mr.mul(ad, ad);
        }
        if (t == s2) return false;
    }
    return true;
}
bool is_prime_montgomery(long long n) {
    if (n == 1) return false;
    if (n == 2) return true;
    if ((n & 1) == 0) return false;
    if (n < 4759123141) return miller_rabin_montgomery(n, {2, 7, 61});
    return miller_rabin_montgomery(n, {2, 325, 9375, 28178, 450775, 9780504, 1795265022});
}