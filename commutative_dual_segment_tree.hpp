template <class F, auto composition, auto id>
class CommutativeDualSegmentTree {
    static_assert(is_convertible_v<decltype(composition), function<F(F, F)>>, "composition must work as F(F, F)");
    static_assert(is_convertible_v<decltype(id), function<F()>>, "id must work as F()");

   public:
    CommutativeDualSegmentTree() : CommutativeDualSegmentTree(0) {}
    explicit CommutativeDualSegmentTree(int n) : CommutativeDualSegmentTree(vector<F>(n, id())) {}
    explicit CommutativeDualSegmentTree(vector<F> f) : n(int(f.size())) {
        h = ceil_log2(n);
        siz = (1 << h);
        act = vector<F>(2 * siz, id());
        copy(f.begin(), f.end(), act.begin() + siz);
    }

    F get(int p) {
        assert(0 <= p && p <= n);
        F f = id();
        p += siz;
        for (int i = 0; i <= h; ++i) f = composition(act[p >> i], f);
        return f;
    }

    void apply(int l, int r, F f) {
        assert(0 <= l && l <= r && r <= n);
        if (l == r) return;
        l += siz;
        r += siz;

        while (l < r) {
            if (l & 1) apply(l++, f);
            if (r & 1) apply(--r, f);
            l >>= 1;
            r >>= 1;
        }
    }

   private:
    int n, h, siz;
    vector<F> act;

    void apply(int k, F f) { act[k] = composition(f, act[k]); }

    int ceil_log2(int x) {
        int res = 0;
        while ((1 << res) < x) ++res;
        return res;
    }
};