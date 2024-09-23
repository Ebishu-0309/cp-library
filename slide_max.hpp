// res[i] = op (i-k, i]
template <class S, auto op>
vector<S> slide_max(vector<S> a, int k) {
    static_assert(is_convertible_v<decltype(op), function<S(S, S)>>, "op must work as S(S, S)");
    int n = a.size();
    deque<pair<int, S>> deq;
    vector<S> res(n);
    for (int i = 0; i < n; ++i) {
        while (!deq.empty() && deq.front().first <= i - k) deq.pop_front();
        while (!deq.empty() && op(deq.back().second, a[i]) == a[i]) deq.pop_back();
        deq.emplace_back(i, a[i]);
        res[i] = deq.front().second;
    }
    return res;
}