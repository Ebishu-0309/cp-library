#include "formal_power_series.hpp"
vector<mint> stirling_second_k(int k, int nmax) {
    const int m = nmax - k;
    FPS f(m + 1);

    auto fact_inv = enumerate_factinv<Mod>(nmax);
    for (int i = 0; i <= m; ++i) {
        f[i] = fact_inv[i + 1];
    }
    f.pow_inplace(k);

    vector<mint> s(m + 1);
    mint fact_n = 1;
    for (int i = 1; i <= k; ++i) fact_n *= i;

    for (int n = k; n <= nmax; ++n) {
        s[n - k] = f[n - k] * fact_n * fact_inv[k];
        fact_n *= (n + 1);
    }
    return s;
}

vector<mint> partition_number(int n) {
    auto inv = enumerate_inv<Mod>(n);
    FPS f(n + 1);
    for (int i = 1; i <= n; ++i) {
        for (int j = 1, lim = n / i; j <= lim; ++j) {
            f[i * j] += inv[j];
        }
    }
    f.exp_inplace();
    return f.as_vector();
}