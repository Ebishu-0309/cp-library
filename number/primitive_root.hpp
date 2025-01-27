#include "pollard_rho.hpp"

long long primitive_root(long long p) {
    if (p == 2) return 1;
    if (p == 3) return 2;
    if (p == 5) return 2;

    assert(is_prime(p));

    vector<long long> es;
    long long m = p - 1;
    if ((m & 1) == 0) {
        es.push_back((p - 1) / 2);
    }
    while ((m & 1) == 0) {
        m >>= 1;
    }
    while (m > 1) {
        long long pi = (is_prime(m) ? m : find_prime_factor(m));
        es.push_back((p - 1) / pi);
        while (m % pi == 0) {
            m /= pi;
        }
    }

    long long g = 2;
    for (long long i = 2;; ++i) {
        if (p > 1000) {
            g = splitmix64(i) % p;
            if (g == 0) continue;
        } else {
            g = i;
        }

        bool is = true;
        for (long long e : es) {
            __int128_t po = 1, ge = g;
            while (e) {
                if (e & 1) po = po * ge % p;
                ge = ge * ge % p;
                e >>= 1;
            }
            if (po == 1) {
                is = false;
                break;
            }
        }
        if (is) {
            return g;
        }
    }
}