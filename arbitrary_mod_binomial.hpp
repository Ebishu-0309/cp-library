class ArbitraryModBinomial {
   public:
    explicit ArbitraryModBinomial(int mod) {
        for (int i = 2; i * i <= mod; ++i) {
            if (mod % i != 0) continue;

            int cnt = 0, prod = 1;
            while (mod % i == 0) {
                ++cnt;
                mod /= i;
                prod *= i;
            }

            p.push_back(i);
            q.push_back(cnt);
            m.push_back(prod);
        }

        if (mod != 1) {
            p.push_back(mod);
            q.push_back(1);
            m.push_back(mod);
        }

        siz = p.size();
        fac.resize(siz);

        for (int i = 0; i < siz; ++i) {
            fac[i].resize(m[i]);
            fac[i][0] = 1;
            for (int j = 1; j < m[i]; ++j) {
                if (j % p[i] == 0)
                    fac[i][j] = fac[i][j - 1];
                else
                    fac[i][j] = fac[i][j - 1] * j % m[i];
            }
        }
    }

    long long calc(long long n, long long r) const {
        vector<long long> rs(siz), ms(siz);
        for (int i = 0; i < siz; ++i) {
            rs[i] = calc_sub(n, r, i);
            ms[i] = m[i];
        }
        return atcoder::crt(rs, ms).first;
    }

   private:
    int siz;
    vector<int> p, q, m;  // m[i] = p[i]^q[i]

    vector<vector<long long>> fac;

    long long calc_sub(long long n, long long r, int i) const {
        const long long k = n - r;
        auto ns = div_list(n, p[i]), rs = div_list(r, p[i]), ks = div_list(k, p[i]);
        const long long e0 = e(ns, rs, ks, 0), e1 = e(ns, rs, ks, q[i] - 1);

        long long comb = (p[i] == 2 && q[i] >= 3 ? 1 : (e1 % 2 == 0 ? 1 : m[i] - 1));
        const int l = max({ns.size(), rs.size(), ks.size()});
        ns.resize(l);
        rs.resize(l);
        ks.resize(l);
        for (int j = 0; j < l; ++j) {
            comb *= fac[i][ns[j] % m[i]] * modinv(fac[i][rs[j] % m[i]] * fac[i][ks[j] % m[i]], m[i]) % m[i];
            comb %= m[i];
        }
        return comb * modpow(p[i], e0, m[i]) % m[i];
    }

    long long e(const vector<long long> &ns, const vector<long long> &rs, const vector<long long> &ks, int j) const {
        long long res = 0;
        const int n_siz = ns.size(), r_siz = rs.size(), k_siz = ks.size();
        for (int i = j + 1; i < n_siz; ++i) res += ns[i];
        for (int i = j + 1; i < r_siz; ++i) res -= rs[i];
        for (int i = j + 1; i < k_siz; ++i) res -= ks[i];
        return res;
    }

    long long modpow(long long x, long long k, long long m) const {
        long long res = 1;
        x %= m;
        while (k > 0) {
            if (k & 1) res = res * x % m;
            x = x * x % m;
            k >>= 1;
        }
        return res;
    }

    long long modinv(long long a, long long m) const {
        long long b = m, u = 1, v = 0;
        while (b != 0) {
            const long long t = a / b;
            a -= t * b;
            u -= t * v;
            swap(a, b);
            swap(u, v);
        }
        u %= m;
        if (u < 0) u += m;
        return u;
    }

    // res[i] = x/(p^i)
    vector<long long> div_list(long long x, int p) const {
        if (x == 0) return {0};
        vector<long long> res;
        long long prod = 1;
        while (true) {
            res.push_back(x / prod);
            if (x / p >= prod)
                prod *= p;
            else
                break;
        }
        return res;
    }
};