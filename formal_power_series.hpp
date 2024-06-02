template <int MOD>
struct FormalPowerSeries : vector<atcoder::static_modint<MOD>> {
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

        for (int i = 2; i < n; ++i) (*this)[i] *= i;
        this->erase(this->begin());
        this->push_back(0);

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

    template <typename U>
    vector<Fp> eval(const vector<U> &x) const {
        const int m1 = x.size();

        if (m1 == 1) return {eval(x[0])};

        int m = 1;
        while (m < m1) m <<= 1;

        vector<F> subproducts(2 * m, F{1});

        for (int i = m; i < m + m1; ++i) {
            subproducts[i].resize(2);
            subproducts[i][0] = -x[i - m];
            subproducts[i][1] = 1;
        }

        for (int i = m - 1; i > 1; --i) {
            subproducts[i] = subproducts[i << 1] * subproducts[i << 1 | 1];
        }

        vector<F> rem(2 * m);
        rem[1] = *this;

        for (int i = 1; i < m; ++i) {
            rem[i << 1] = rem[i] % subproducts[i << 1];
            rem[i << 1 | 1] = rem[i] % subproducts[i << 1 | 1];
        }

        vector<Fp> res(m1);

        for (int i = 0; i < m1; ++i) {
            if (!rem[i + m].empty()) res[i] = rem[i + m][0];
        }

        return res;
    }

    friend F operator*(const Fp &k, const F &f) noexcept { return f * k; }

    static F prod(vector<F> fs) {
        const int n = fs.size();
        if (n == 0) return {1};

        for (int i = n - 1; i > 0; --i) {
            fs[i / 2] = convolution(fs[i / 2], fs[i]);
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

    // O(sqrt(MOD) log^2 MOD)
    static Fp factorial(long long n) {
        if (n >= MOD) return 0;
        static constexpr int v = 1 << 15;  // v * v >= MOD
        static vector<int> b(v);
        iota(b.begin(), b.end(), 1);
        static const F f = prod(vector<int>(v, 1), b);  // prod_1^v (x + i)

        const int m = min<int>(v, (n + 1) / v);
        vector<int> xs(m);
        for (int i = 0; i < m; ++i) xs[i] = v * i;

        Fp p = 1;
        for (const auto e : f.eval(xs)) p *= e;
        for (int i = m * v + 1; i <= n; ++i) p *= i;
        return p;
    }

    static void coeff_sub(vector<mint> &a, bool odd) {
        const int n = a.size();
        for (int i = odd; i < n; i += 2) a[i >> 1] = a[i];
        a.resize((n + !odd) >> 1);
    }

    constexpr unsigned int bitceil(int n) { return atcoder::internal::bit_ceil((unsigned int)(2 * n - 1)); }

    // [x^k] (p/q)
    static Fp coeff(vector<mint> p, vector<mint> q, long long k) {
        vector<mint> q_dft;
        {
            const int n0 = q.size();
            p.resize(n0);
            const int z = bitceil(n0);
            p.resize(z);
            q.resize(z);
            q_dft = q;
            atcoder::internal::butterfly(q_dft);
        }
        while (k > 0) {
            const int n = q.size();
            const int z = bitceil(n);

            p.resize(z);
            q.resize(z);
            atcoder::internal::butterfly(p);
            atcoder::internal::butterfly(q);

            vector<mint> q_minus_dft = q;
            for (int i = 0; i < z; ++i) p[i] *= q_minus_dft[i ^ 1], q[i] *= q_minus_dft[i ^ 1];
            atcoder::internal::butterfly_inv(p);
            atcoder::internal::butterfly_inv(q);

            p.resize(2 * n - 1);
            q.resize(2 * n - 1);

            coeff_sub(p, k & 1);
            coeff_sub(q, 0);
            k >>= 1;
        }
        return p[0] / q[0];
    }

    static Fp kth_term(const vector<mint> &a, const vector<mint> &c, long long k) {
        const int d = a.size();
        vector<mint> q(d + 1);
        q[0] = 1;
        for (int i = 0; i < d; ++i) q[i + 1] = -c[i];
        vector<mint> p = convolution(a, q);
        p.resize(d);
        return coeff(p, q, k);
    }
};
using FPS = FormalPowerSeries<Mod>;
struct Comp {
    bool operator()(const FPS &a, const FPS &b) const { return a.size() > b.size(); }
};