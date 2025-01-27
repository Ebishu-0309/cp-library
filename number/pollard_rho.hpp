#include "miller_rabin.hpp"
#include "../hash/splitmix64.hpp"

// n must not be prime
long long find_prime_factor(long long n) {
    constexpr int m = 200;
    long long c = 1;
    while (true) {
        c = splitmix64(c) % n;
        if (c == 0) continue;
        auto f = [=](long long x) { return (__int128_t(x) * x + c) % n; };
        long long x = 0, y = 0, y_b = 0, q = 1;
        long long g = 1;
        int r = 1, k = 0;
        do {
            x = y;
            while (k < 3 * r / 4) {
                y = f(y);
                ++k;
            }
            while (k < r && g == 1) {
                y_b = y;
                for (int i = 0; i < min(m, r - k); ++i) {
                    y = f(y);
                    q = __int128_t(q) * abs(x - y) % n;
                }
                g = gcd(q, n);
                k += m;
            }
            k = r;
            r <<= 1;
        } while (g == 1);

        if (g == n) {
            g = 1;
            y = y_b;
            do {
                y = f(y);
                g = gcd(abs(x - y), n);
            } while (g == 1);
        }
        if (g == n) continue;

        if (is_prime(g)) return g;
        if (is_prime(n / g)) return n / g;
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
                long long p = find_prime_factor(n);
                do {
                    ps.push_back(p);
                    n /= p;
                } while (n % p == 0);

                if (is_prime(n)) {
                    ps.push_back(n);
                    break;
                }
            }
        }
    }
    sort(ps.begin(), ps.end());
    return ps;
}