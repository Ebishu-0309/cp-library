template <typename Cap, typename Cost>
vector<Cost> interpolate_slope(vector<pair<Cap, Cost>> slope) {
    vector<Cost> res(slope.back().first + 1);

    int n = slope.size();
    for (auto [cap, cost] : slope) res[cap] = cost;
    for (int i = 0; i < n - 1; ++i) {
        int l = slope[i].first, r = slope[i + 1].first;
        for (int j = l + 1; j < r; ++j) {
            res[j] = (res[l] * (r - j) + res[r] * (j - l)) / (r - l);
        }
    }

    return res;
}