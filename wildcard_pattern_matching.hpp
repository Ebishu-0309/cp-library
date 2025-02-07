vector<bool> wildcard_pattern_matching(string s, string word, char wild = '*') {
    static mt19937 mt(random_device{}());
    static uniform_int_distribution<int> dist(0, mint::mod() - 1);

    int n = s.size(), m = word.size();
    if (n < m) return {};
    int z = atcoder::internal::bit_ceil((unsigned int)(n));

    reverse(s.begin(), s.end());
    vector<mint> t1(z), t2(z), s2(z);
    for (int i = 0; i < m; ++i) {
        if (word[i] != wild) {
            const mint r = dist(mt);
            t1[i] = r;
            t2[i] = r * (word[i] - 'a');
        }
    }
    atcoder::internal::butterfly(t1);
    atcoder::internal::butterfly(t2);

    for (int i = 0; i < n; ++i) s2[i] = (s[i] == wild ? 0 : (s[i] - 'a'));
    atcoder::internal::butterfly(s2);
    for (int i = 0; i < z; ++i) t1[i] *= s2[i];
    for (int i = 0; i < n; ++i) s2[i] = s[i] != wild;
    atcoder::internal::butterfly(s2);
    for (int i = 0; i < z; ++i) t1[i] -= t2[i] * s2[i];
    atcoder::internal::butterfly_inv(t1);

    vector<bool> ans(n - m + 1);
    for (int k = 0; k <= n - m; ++k) ans[k] = (t1[n - 1 - k] == 0);

    return ans;
}