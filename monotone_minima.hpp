template <typename T, class F, class Comp = less<T>>
vector<pair<int, T>> monotone_minima(int h, int w, F f, Comp comp = Comp{}) {
    vector<pair<int, T>> res(h);
    auto dfs = [&](auto self, int t, int b, int l, int r) -> void {
        if (t >= b) return;
        int m = (t + b) / 2;
        T f_min = f(m, l);
        int argmin = l;
        for (int j = l + 1; j < r; ++j) {
            const T fj = f(m, j);
            if (comp(fj, f_min)) {
                argmin = j;
                f_min = fj;
            }
        }
        res[m] = {argmin, f_min};
        self(self, t, m, l, argmin + 1);
        self(self, m + 1, b, argmin, r);
    };
    dfs(dfs, 0, h, 0, w);
    return res;
}