#include "formal_power_series.hpp"

vector<bool> wildcard_pattern_matching(string s, string word, char wild = '*') {
    int n = s.size(), m = word.size();
    if (n < m) return {};

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

    auto c1 = FPS::middle_product(t1, s1);
    auto c2 = FPS::middle_product(t2, s2);
    auto c3 = FPS::middle_product(t3, s3);

    vector<bool> ans(n - m + 1);
    for (int k = 0; k <= n - m; ++k) ans[k] = (c1[k] - 2 * c2[k] + c3[k] == 0);

    return ans;
}