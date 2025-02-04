#include <atcoder/convolution>

template <int MOD>
struct FormalPowerSeries : public vector<atcoder::static_modint<MOD>> {
    using Fp = atcoder::static_modint<MOD>;

    using vector<Fp>::vector;
    using vector<Fp>::operator=;
    using F = FormalPowerSeries;

    FormalPowerSeries(const vector<Fp> &vec) { *this = vec; }

    void shrink() {
        while (!this->empty() && this->back() == 0) this->pop_back();
    }

    F operator+() const noexcept { return *this; }
    F operator-() const noexcept {
        F res(*this);

        for (auto &&e : res) e = -e;

        return res;
    }

    F operator*(const Fp &k) const noexcept { return F(*this) *= k; }
    F operator/(const Fp &k) const { return F(*this) /= k; }

    F operator+(const F &g) const noexcept { return F(*this) += g; }
    F operator-(const F &g) const noexcept { return F(*this) -= g; }

    F operator<<(const int d) const noexcept { return F(*this) <<= d; }
    F operator>>(const int d) const noexcept { return F(*this) >>= d; }

    F operator*(const F &g) const { return F(*this) *= g; }
    F operator/(const F &g) const { return F(*this) /= g; }
    F operator%(const F &g) const { return F(*this) %= g; }

    F &operator*=(const Fp &k) noexcept {
        for (auto &&e : *this) e *= k;
        return *this;
    }
    F &operator/=(const Fp &k) {
        assert(k != 0);
        *this *= k.inv();
        return *this;
    }

    F &operator+=(const F &g) noexcept {
        const int n = this->size(), m = g.size();
        this->resize(max(n, m));

        for (int i = 0; i < m; ++i) (*this)[i] += g[i];

        return *this;
    }
    F &operator-=(const F &g) noexcept {
        const int n = this->size(), m = g.size();
        this->resize(max(n, m));

        for (int i = 0; i < m; ++i) (*this)[i] -= g[i];

        return *this;
    }

    F &operator<<=(const int d) {
        const int n = this->size();

        this->insert(this->begin(), d, Fp(0));

        return *this;
    }
    F &operator>>=(const int d) {
        const int n = this->size();

        if (n <= d)
            this->clear();
        else
            this->erase(this->begin(), this->begin() + d);

        return *this;
    }

    F &operator*=(const F &g) {
        const auto f = atcoder::convolution(std::move(*this), g);
        return *this = F(f.begin(), f.end());
    }
    F &operator/=(const F &g) {
        if (this->size() < g.size()) {
            this->clear();
            return *this;
        }
        const int n = this->size() - g.size() + 1;
        return *this = (rev().pre(n) * g.rev().inv(n)).pre(n).rev(n);
    }
    F &operator%=(const F &g) {
        *this -= *this / g * g;

        this->shrink();

        return *this;
    }

    bool zero() const noexcept {
        bool res = true;
        for (const auto &e : *this) {
            res &= (e.val() == 0);
        }
        return res;
    }

    Fp eval(const Fp &x) const noexcept {
        Fp res = this->back();

        for (auto itr = ++this->rbegin(), itr_rend = this->rend(); itr != itr_rend; ++itr) {
            res *= x;
            res += *itr;
        }

        return res;
    }

    F pre(int d) const { return F(this->begin(), this->begin() + min((int)this->size(), d)); }
    F rev(int d = -1) const {
        F res(*this);
        if (d != -1) res.resize(d, Fp(0));
        reverse(res.begin(), res.end());
        return res;
    }

    F inv(int d = -1) const {
        int n = this->size();
        assert(n != 0 && this->front() != Fp(0));
        if (d == -1) d = n;
        assert(d > 0);

        F res = {1 / this->front()};
        res.reserve(2 * d);

        int m = res.size();
        while (m < d) {
            F f(this->begin(), this->begin() + min(n, 2 * m));
            F r(res);

            f.resize(2 * m);
            r.resize(2 * m);
            atcoder::internal::butterfly(f);
            atcoder::internal::butterfly(r);
            for (int i = 0; i < 2 * m; ++i) f[i] *= r[i];
            atcoder::internal::butterfly_inv(f);

            f.erase(f.begin(), f.begin() + m);
            f.resize(2 * m);
            atcoder::internal::butterfly(f);
            for (int i = 0; i < 2 * m; ++i) f[i] *= r[i];
            atcoder::internal::butterfly_inv(f);

            Fp iz = Fp(1) / (2 * m);
            iz *= -iz;

            for (int i = 0; i < m; ++i) f[i] *= iz;

            res.insert(res.end(), f.begin(), f.begin() + m);

            m <<= 1;
        }

        return {res.begin(), res.begin() + d};
    }

    F &multiply_inplace(const F &g, int d = -1) {
        if (d == -1) d = this->size();
        assert(d >= 0);

        *this = atcoder::convolution(move(*this), g);
        this->resize(d);

        return *this;
    }
    F multiply(const F &g, int d = -1) const { return F(*this).multiply_inplace(g, d); }

    F &diff_inplace() noexcept {
        const int n = this->size();

        for (int i = 1; i < n; ++i) (*this)[i - 1] = (*this)[i] * i;
        this->back() = 0;

        return *this;
    }
    F diff() const noexcept { return F(*this).diff_inplace(); }

    F &integral_inplace() noexcept {
        constexpr int p = Fp::mod();
        const int n = this->size();

        vector<Fp> inv(n);
        inv[1] = 1;
        for (int i = 2; i < n; ++i) inv[i] = -inv[p % i] * (p / i);

        for (int i = n - 1; i > 0; --i) (*this)[i] = (*this)[i - 1] * inv[i];

        this->front() = 0;

        return *this;
    }
    F integral() const noexcept { return F(*this).integral_inplace(); }

    F &log_inplace(int d = -1) {
        const int n = this->size();
        assert(this->front() == 1);
        if (d == -1) d = n;
        assert(d >= 0);

        this->resize(d);

        const F f_inv = this->inv();

        this->diff_inplace().multiply_inplace(f_inv).integral_inplace();

        return *this;
    }
    F log(int d = -1) const { return F(*this).log_inplace(d); }

    F &exp_inplace(int d = -1) {
        const int n = this->size();
        assert(this->front() == 0);
        if (d == -1) d = n;
        assert(d >= 0);

        F g = {1}, g_fft;

        this->resize(d);
        this->front() = 1;

        const F h_diff = this->diff();
        for (int m = 1; m < d; m <<= 1) {
            F f_fft(this->begin(), this->begin() + m);
            f_fft.resize(2 * m);
            atcoder::internal::butterfly(f_fft);
            if (m > 1) {
                F _f(m);
                for (int i = 0; i < m; ++i) _f[i] = f_fft[i] * g_fft[i];
                atcoder::internal::butterfly_inv(_f);
                _f.erase(_f.begin(), _f.begin() + m / 2);
                _f.resize(m);
                atcoder::internal::butterfly(_f);
                for (int i = 0; i < m; ++i) _f[i] *= g_fft[i];
                atcoder::internal::butterfly_inv(_f);
                _f.resize(m / 2);

                _f /= Fp(-m) * m;
                g.insert(g.end(), _f.begin(), _f.begin() + m / 2);
            }

            F t(this->begin(), this->begin() + m);
            t.diff_inplace();
            {
                F r(h_diff.begin(), h_diff.begin() + (m - 1));

                r.resize(m);
                atcoder::internal::butterfly(r);
                for (int i = 0; i < m; ++i) r[i] *= f_fft[i];
                atcoder::internal::butterfly_inv(r);
                r /= -m;

                t += r;
                t.insert(t.begin(), t.back());
                t.pop_back();
            }

            if (2 * m < d || m == 1) {
                t.resize(2 * m);
                atcoder::internal::butterfly(t);
                g_fft = g;
                g_fft.resize(2 * m);
                atcoder::internal::butterfly(g_fft);
                for (int i = 0; i < 2 * m; ++i) t[i] *= g_fft[i];
                atcoder::internal::butterfly_inv(t);
                t.resize(m);
                t /= 2 * m;
            } else {
                F g1(g.begin() + m / 2, g.end());
                F s1(t.begin() + m / 2, t.end());
                t.resize(m / 2);
                g1.resize(m);
                atcoder::internal::butterfly(g1);
                t.resize(m);
                atcoder::internal::butterfly(t);
                s1.resize(m);
                atcoder::internal::butterfly(s1);
                for (int i = 0; i < m; ++i) s1[i] = g_fft[i] * s1[i] + g1[i] * t[i];
                for (int i = 0; i < m; ++i) t[i] *= g_fft[i];
                atcoder::internal::butterfly_inv(t);
                atcoder::internal::butterfly_inv(s1);
                for (int i = 0; i < m / 2; ++i) t[i + m / 2] += s1[i];
                t /= m;
            }

            F v(this->begin() + m, this->begin() + min(d, 2 * m));
            v.resize(m);
            t.insert(t.begin(), m - 1, 0);
            t.push_back(0);
            t.integral_inplace();
            for (int i = 0; i < m; ++i) v[i] -= t[m + i];

            v.resize(2 * m);
            atcoder::internal::butterfly(v);
            for (int i = 0; i < 2 * m; ++i) v[i] *= f_fft[i];
            atcoder::internal::butterfly_inv(v);
            v.resize(m);
            v /= 2 * m;

            for (int i = 0; i < min(d - m, m); ++i) (*this)[m + i] = v[i];
        }

        return *this;
    }
    F exp(int d = -1) const { return F(*this).exp_inplace(d); }

    F &pow_inplace(long long m, long long d = -1) {
        const long long n = this->size();
        if (d == -1) d = n;
        assert(d > 0);

        if (m == 0) {
            F res(d);
            res[0] = 1;
            return *this = res;
        }
        if (zero()) {
            return *this = F(d);
        }

        long long k = 0;
        while (k < n && (*this)[k] == 0) ++k;
        if (k >= (d + m - 1) / m) return *this = F(d);

        const Fp c_inv = (*this)[k].inv();
        const Fp c_pow = (*this)[k].pow(m);

        this->erase(this->begin(), this->begin() + k);
        *this *= c_inv;
        this->log_inplace(d);
        *this *= m;
        this->exp_inplace(d);
        *this *= c_pow;

        this->insert(this->begin(), m * k, 0);
        this->resize(d);

        return *this;
    }
    F pow(long long m, int d = -1) const { return F(*this).pow_inplace(m, d); }

    // a.size() = n, c.size() = n + m - 1
    // res[j] = sum a[i] * c[i + j] (0 <= j < m)
    static vector<Fp> middle_product(vector<mint> a, vector<mint> c) {
        int n = a.size(), m = c.size() + 1 - n;
        if (m <= 0) return {};
        if (min(n, m) <= 60) return middle_product_naive(a, c);
        return middle_product_fft(a, c);
    }

    template <typename U>
    vector<Fp> eval(const vector<U> &x) const {
        const int n = this->size();
        const int m1 = x.size();

        if (m1 == 1) return {eval(x[0])};

        int m = 1;
        while (m < m1) m <<= 1;

        vector t(2 * m, vector<Fp>{1});
        for (int i = m; i < m + m1; ++i) {
            t[i].resize(2);
            t[i][0] = -x[i - m];
            t[i][1] = 1;
        }
        for (int i = m - 1; i >= 1; --i) t[i] = atcoder::convolution(t[i << 1], t[i << 1 | 1]);

        F t1 = F(t[1]).rev().inv(n);

        vector<Fp> f(*this);
        f.resize(n + m1 - 1);

        vector<Fp> a = middle_product(t1, f);
        reverse(a.begin(), a.end());

        vector b(2 * m, vector<Fp>{});
        b[1] = a;

        for (int i = 1; i < m; ++i) {
            b[i << 1 | 1] = middle_product(t[i << 1], b[i]);
            b[i << 1] = middle_product(t[i << 1 | 1], b[i]);
        }

        vector<mint> res(m1);
        for (int i = m; i < m + m1; ++i) res[i - m] = b[i][0];

        return res;
    }

    friend F operator*(const Fp &k, const F &f) noexcept { return f * k; }

    static F prod(vector<F> fs) {
        const int n = fs.size();
        if (n == 0) return {1};

        for (int i = n - 1; i > 0; --i) {
            fs[i / 2] = atcoder::convolution(fs[i / 2], fs[i]);
        }
        return fs[0];
    }

    // prod (a_i x + b_i)
    template <typename U, typename V>
    static F prod(const vector<U> &a, const vector<V> &b) {
        const size_t n = a.size();

        vector<F> fs(n, F{0, 0});
        for (size_t i = 0; i < n; ++i) {
            fs[i][0] = b[i];
            fs[i][1] = a[i];
        }
        return prod(fs);
    }

    // pre: O(sqrt(MOD) log^2 MOD)
    // O(sqrt(MOD))
    static Fp factorial(long long n) {
        if (n >= MOD) return 0;
        static constexpr int v = 1 << 15;  // v * v >= MOD
        static vector<Fp> ps(v + 1);       // ps[i] = prod_1^{iv} j

        static bool init = false;
        if (!init) {
            init = true;
            vector<int> k(v);
            iota(k.begin(), k.end(), 1);
            const F f = prod(vector<int>(v, 1), k);  // (x+1)(x+2)...(x+v)
            vector<int> xs(v);
            for (int i = 0; i < v; ++i) xs[i] = v * i;
            vector<Fp> es = f.eval(xs);
            ps[0] = 1;
            for (int i = 0; i < v; ++i) ps[i + 1] = ps[i] * es[i];
        }
        const int m = min<int>(v, (n + 1) / v);

        Fp p = ps[m];
        for (int i = m * v + 1; i <= n; ++i) p *= i;
        return p;
    }

    // dft.size() == 2 * n, dft[0 : n] = DFT(f)
    // dft <- DFT(f + [0] * n)
    // time complexity: FFT(n)
    static void fft_doubling(vector<mint> &dft, vector<mint> f, const mint r_2n) {
        const int n = dft.size() >> 1;
        mint rp = 1;
        for (auto &e : f) {
            e *= rp;
            rp *= r_2n;
        }
        atcoder::internal::butterfly(f);
        copy(f.begin(), f.end(), dft.begin() + n);
    }

    // dft.size() == 2 * n
    // dft <- DFT(IDFT(dft[0 : n]) + [0] * n)
    // time complexity: 2 FFT(n)
    static void fft_doubling(vector<mint> &dft, const mint r, const mint n_inv) {
        const int n = dft.size() >> 1;

        vector<mint> b(n);
        copy(dft.begin(), dft.begin() + n, b.begin());
        atcoder::internal::butterfly_inv(b);
        mint rp = 1;
        for (auto &e : b) {
            e *= rp * n_inv;
            rp *= r;
        }
        atcoder::internal::butterfly(b);
        copy(b.begin(), b.end(), dft.begin() + n);
    }

    // [x^k] (p/q) (deg p < deg q)
    static Fp coeff(vector<Fp> p, vector<Fp> q, long long k) {
        static const atcoder::internal::fft_info<Fp> info;
        static const Fp inv2 = Fp::raw((Fp::mod() + 1) / 2);

        const int n = atcoder::internal::bit_ceil((unsigned int)(q.size()));

        p.resize(2 * n), q.resize(2 * n);
        atcoder::internal::butterfly(p);
        atcoder::internal::butterfly(q);

        const int w = __builtin_ctz((unsigned int)(n));
        const mint n_inv = mint::raw(n).inv();
        const mint r_z = info.root[w + 1];
        const Fp ir_z = info.iroot[w + 1];

        vector<int> bit_reverse(n);
        vector<Fp> ir_p(n);
        for (int i = 0; i < n; ++i) {
            bit_reverse[i] = (bit_reverse[i >> 1] >> 1) | ((i & 1) << (w - 1));
            ir_p[i] = ir_z.pow(bit_reverse[i]);
        }

        Fp inv2_p = 1;
        while (k > 0) {
            inv2_p *= inv2;

            if (k & 1) {
                for (int i = 0; i < n; ++i) p[i] = ir_p[i] * (p[i << 1] * q[i << 1 | 1] - p[i << 1 | 1] * q[i << 1]);
            } else {
                for (int i = 0; i < n; ++i) p[i] = p[i << 1] * q[i << 1 | 1] + p[i << 1 | 1] * q[i << 1];
            }
            for (int i = 0; i < n; ++i) q[i] = q[i << 1] * q[i << 1 | 1];

            fft_doubling(p, r_z, n_inv);
            fft_doubling(q, r_z, n_inv);

            k >>= 1;
        }

        Fp p0 = 0, q0 = 0;
        for (auto e : p) p0 += e;
        for (auto e : q) q0 += e;

        return inv2_p * p0 / q0;
    }

    static Fp kth_term(const vector<Fp> &a, const vector<Fp> &c, long long k) {
        const int d = a.size();
        vector<Fp> q(d + 1);
        q[0] = 1;
        for (int i = 0; i < d; ++i) q[i + 1] = -c[i];
        vector<Fp> p = atcoder::convolution(a, q);
        p.resize(d);
        return coeff(p, q, k);
    }

    static vector<Fp> middle_product_naive(vector<mint> a, vector<mint> c) {
        int n = a.size(), m = c.size() + 1 - n;
        vector<mint> b(m);
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                b[j] += a[i] * c[i + j];
            }
        }
        return b;
    }
    static vector<Fp> middle_product_fft(vector<mint> a, vector<mint> c) {
        int n = a.size(), m = c.size() + 1 - n;
        reverse(c.begin(), c.end());

        int z = atcoder::internal::bit_ceil((unsigned int)(n + m));
        a.resize(z), c.resize(z);
        atcoder::internal::butterfly(a), atcoder::internal::butterfly(c);
        for (int i = 0; i < z; ++i) a[i] *= c[i];
        atcoder::internal::butterfly_inv(a);
        a.resize(n + m - 1);
        a.erase(a.begin(), a.begin() + n - 1);
        reverse(a.begin(), a.end());
        const mint iz = mint(z).inv();
        for (auto &e : a) e *= iz;
        return a;
    }
};

using FPS = FormalPowerSeries<Mod>;
struct Comp {
    bool operator()(const FPS &a, const FPS &b) const { return a.size() > b.size(); }
};