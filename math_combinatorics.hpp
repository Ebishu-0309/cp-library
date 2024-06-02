vector<int> linear_sieve(int n) {
    vector<int> min_p(n + 1);
    vector<int> ps;
    ps.reserve(n);
    for (int d = 2; d <= n; ++d) {
        if (min_p[d] == 0) {
            min_p[d] = d;
            ps.push_back(d);
        }
        for (auto p : ps) {
            if (p * d > n || p > min_p[d]) break;
            min_p[d * p] = p;  // p <= min_p[d]
        }
    }
    return min_p;
}

vector<mint> stirling_second_n(int n) {
    auto inv = enumerate_inv<Mod>(n + 1);

    vector<int> min_p = linear_sieve(n);
    min_p[1] = 1;
    vector<mint> pow_n(n + 1);
    pow_n[0] = (n == 0 ? 1 : 0);
    for (int i = 1; i <= n; ++i) {
        int p1 = min_p[i];
        if (p1 == i)
            pow_n[i] = mint::raw(i).pow(n);
        else
            pow_n[i] = pow_n[p1] * pow_n[i / p1];
    }

    vector<mint> a(n + 1), b(n + 1);
    mint fact_inv = 1;
    for (int i = 0; i <= n; ++i) {
        if (i > 0) fact_inv *= inv[i];
        a[i] = ((i & 1) ? -fact_inv : fact_inv);
        b[i] = pow_n[i] * fact_inv;
    }

    a = convolution(a, b);
    a.resize(n + 1);
    return a;
}

template <typename T>
vector<T> montmort_number(int n) {
    vector<T> a(n);
    a[0] = 0;
    for (int i = 1; i < n; ++i) {
        a[i] = (i + 1) * a[i - 1];
        if (i & 1)
            a[i]++;
        else
            a[i]--;
    }
    return a;
}

template <typename T>
mint number_of_subsequences(vector<T> a) {
    const int n = a.size();
    const vector<int> ind = compressed_index(a);
    vector<int> pre(n);
    vector<mint> sum(n + 2);
    sum[1] = 1;
    for (int i = 0; i < n; ++i) {
        sum[i + 2] = sum[i + 1] + (sum[i + 1] - sum[pre[ind[i]]]);
        pre[ind[i]] = i + 1;
    }
    return sum[n + 1] - 1;
}