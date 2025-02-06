template <typename T>
class StaticRangeModeQuery {
    int n, m;
    vector<int> a;
    vector<T> a_sorted;
    int b, c;
    vector<vector<pair<T, int>>> pre;
    vector<vector<int>> idx;
    vector<int> idx2;

   public:
    StaticRangeModeQuery(vector<T> a_) : n((int)(a_.size())), a_sorted(a_) {
        sort(a_sorted.begin(), a_sorted.end());
        a_sorted.erase(unique(a_sorted.begin(), a_sorted.end()), a_sorted.end());
        m = a_sorted.size();
        a.resize(n);
        for (int i = 0; i < n; ++i) a[i] = lower_bound(a_sorted.begin(), a_sorted.end(), a_[i]) - a_sorted.begin();

        b = sqrt(n), c = (n + b - 1) / b;
        pre.assign(c + 1, vector<pair<T, int>>(c + 1));
        idx.resize(m);
        idx2.resize(n);
        for (int l = 0; l < n; l += b) {
            vector<int> cnt(m);
            T mode = T{};
            int cnt_max = 0;
            for (int i = l; i < n; ++i) {
                if (i % b == 0) pre[l / b][i / b] = {mode, cnt_max};
                cnt[a[i]]++;
                if (chmax(cnt_max, cnt[a[i]])) mode = a[i];
            }
            if (n % b == 0) pre[l / b][n / b] = {mode, cnt_max};
        }
        for (int i = 0; i < n; ++i) {
            idx2[i] = idx[a[i]].size();
            idx[a[i]].push_back(i);
        }
    }

    pair<T, int> calc(int l, int r) const {
        const int bl = (l + b - 1) / b, br = r / b;
        auto [mode, cnt] = pre[bl][br];
        for (int i = l; i < r && i < bl * b; ++i) {
            while (idx2[i] + cnt < (int)idx[a[i]].size() && idx[a[i]][idx2[i] + cnt] < r) mode = a[i], ++cnt;
        }
        for (int i = r - 1; l <= i && br * b <= i; --i) {
            while (idx2[i] - cnt >= 0 && idx[a[i]][idx2[i] - cnt] >= l) mode = a[i], ++cnt;
        }
        return {a_sorted[mode], cnt};
    }
};