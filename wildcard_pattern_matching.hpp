vector<bool> wildcard_pattern_matching(string s, string word, char wild = '*') {
    int n = s.size(), m = word.size();
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
    reverse(all(s1));
    reverse(all(s2));
    reverse(all(s3));

    auto c1 = convolution(s1, t1);
    auto c2 = convolution(s2, t2);
    auto c3 = convolution(s3, t3);

    vector<bool> ans(n - m + 1);
    for (int k = 0; k <= n - m; ++k) {
        mint prod = c1[n - 1 - k] - 2 * c2[n - 1 - k] + c3[n - 1 - k];
        ans[k] = (prod == 0);
    }
    return ans;
}