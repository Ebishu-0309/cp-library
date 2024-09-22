vector<int> manacher(string s) {
    int n = s.size();
    vector<int> r(n, 1);
    int c = 0;
    for (int i = 0; i < n; ++i) {
        int j = c - (i - c);
        if (i + r[j] < c + r[c]) {
            r[i] = r[j];
        } else {
            int ri = c + r[c] - i;
            while (0 <= i - ri && i + ri < n && s[i - ri] == s[i + ri]) ri++;
            r[i] = ri;
            c = i;
        }
    }
    return r;
}