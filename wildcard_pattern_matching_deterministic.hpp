vector<bool> wildcard_pattern_matching_deterministic(string s, string word, char wild = '*') {
    int n = s.size(), m = word.size();
    if (n < m) return {};

    reverse(s.begin(), s.end());
    vector<mint> s1(n), s2(n), s3(n);
    rep(i, n) {
        s3[i] = (s[i] != wild);
        s2[i] = (s[i] - 'a') * s3[i];
        s1[i] = (s[i] - 'a') * (s[i] - 'a') * s3[i];
    }
    vector<mint> t1(m), t2(m), t3(m);
    rep(i, m) {
        t1[i] = (word[i] != wild);
        t2[i] = (word[i] - 'a') * t1[i];
        t3[i] = (word[i] - 'a') * (word[i] - 'a') * t1[i];
    }

    int z = atcoder::internal::bit_ceil((unsigned int)(n));
    s1.resize(z), s2.resize(z), s3.resize(z);
    t1.resize(z), t2.resize(z), t3.resize(z);
    atcoder::internal::butterfly(s1);
    atcoder::internal::butterfly(s2);
    atcoder::internal::butterfly(s3);
    atcoder::internal::butterfly(t1);
    atcoder::internal::butterfly(t2);
    atcoder::internal::butterfly(t3);
    for (int i = 0; i < z; ++i) {
        t1[i] *= s1[i];
        t1[i] -= 2 * t2[i] * s2[i];
        t1[i] += t3[i] * s3[i];
    }
    atcoder::internal::butterfly_inv(t1);

    vector<bool> ans(n - m + 1);
    for (int k = 0; k <= n - m; ++k) ans[k] = (t1[n - 1 - k] == 0);

    return ans;
}