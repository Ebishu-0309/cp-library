class Montgomery {
   public:
    using u64 = uint64_t;
    using u128 = __uint128_t;

   private:
    u64 mod, inv_minus_mod, r2;

    u64 calc_inv(u64 x) {
        // Newton's Method
        u64 res = x;  // mod 4
        for (int i = 0; i < 5; ++i) res *= 2 - x * res;
        return res;
    }

   public:
    Montgomery(u64 m) { set_mod(m); }

    void set_mod(u64 m) {
        mod = m;
        inv_minus_mod = -calc_inv(m);
        r2 = -u128(mod) % mod;  // -u128(mod) = 2^128 - mod
    }

    u64 reduce(u128 t) const { return (t + u128(u64(t) * inv_minus_mod) * mod) >> 64; }
    u64 get(u64 x) const { return (x < mod ? x : x - mod); }

    u64 mul(u64 a, u64 b) const { return get(reduce((u128)(reduce(u128(a) * b)) * r2)); }

    u128 convert(u64 x) const { return reduce(u128(x) * r2); }
};

bool miller_rabin_montgomery(lint n, const vector<lint> &ts) {
    const Montgomery mr(n);

    const lint s2 = (n - 1) & (1 - n), d = (n - 1) / s2;
    const auto pow_mod_n = [=](lint b, lint p) {
        lint res = 1;
        while (p > 0) {
            if (p & 1) res = mr.mul(res, b);
            b = mr.mul(b, b);
            p >>= 1;
        }
        return res;
    };
    for (const lint a : ts) {
        if (a >= n) break;
        auto ad = pow_mod_n(a, d);
        if (ad == 1) continue;
        lint t = 1;
        for (; t < s2; t <<= 1) {
            if (ad == n - 1) break;
            ad = mr.mul(ad, ad);
        }
        if (t == s2) return false;
    }
    return true;
}
bool is_prime_montgomery(lint n) {
    if (n == 1) return false;
    if (n == 2) return true;
    if ((n & 1) == 0) return false;
    if (n < 4759123141) return miller_rabin_montgomery(n, {2, 7, 61});
    return miller_rabin_montgomery(n, {2, 325, 9375, 28178, 450775, 9780504, 1795265022});
}

bool miller_rabin(lint n, const vector<lint> &ts) {
    const lint s2 = (n - 1) & (1 - n), d = (n - 1) / s2;
    const auto pow_mod_n = [=](lint b, lint p) {
        lint res = 1;
        while (p > 0) {
            if (p & 1) res = __int128_t(res) * b % n;
            b = __int128_t(b) * b % n;
            p >>= 1;
        }
        return res;
    };
    for (const lint a : ts) {
        if (a >= n) break;
        auto ad = pow_mod_n(a, d);
        if (ad == 1) continue;
        lint t = 1;
        for (; t < s2; t <<= 1) {
            if (ad == n - 1) break;
            ad = __int128_t(ad) * ad % n;
        }
        if (t == s2) return false;
    }
    return true;
}
bool is_prime(lint n) {
    if (n == 1) return false;
    if (n == 2) return true;
    if ((n & 1) == 0) return false;
    if (n < 4759123141) return miller_rabin(n, {2, 7, 61});
    return miller_rabin(n, {2, 325, 9375, 28178, 450775, 9780504, 1795265022});
}