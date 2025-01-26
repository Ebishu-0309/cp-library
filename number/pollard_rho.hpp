#include "miller_rabin.hpp"

long long find_prime_factor(long long n) {
    for (long long c = n - 1; c >= 1; --c) {
        auto f = [=](long long x) { return (__int128_t(x) * x + c) % n; };
        long long x = 0, y = 0, g = 1;
        while (g == 1) {
            x = f(x);
            y = f(f(y));
            g = gcd(abs(x - y), n);
        }
        if (g == n) continue;
        if (is_prime(g)) {
            return g;
        }
        if (is_prime(n / g)) {
            return n / g;
        }
        return find_prime_factor(min(n / g, g));
    }
    return n;
}

vector<long long> factorize(long long n) {
    vector<long long> ps;
    if (n >= 2) {
        while ((n & 1) == 0) {
            ps.push_back(2);
            n >>= 1;
        }
        if (is_prime(n)) {
            ps.push_back(n);
        } else {
            while (n > 1) {
                if (is_prime(n)) {
                    ps.push_back(n);
                    break;
                }
                long long p = find_prime_factor(n);
                while (n % p == 0) {
                    ps.push_back(p);
                    n /= p;
                }
            }
        }
    }
    sort(ps.begin(), ps.end());
    return ps;
}