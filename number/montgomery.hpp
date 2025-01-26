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