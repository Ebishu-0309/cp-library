template <class F, auto composition, auto id>
class DualSegmentTree {
    static_assert(is_convertible_v<decltype(composition), function<F(F, F)>>, "composition must work as F(F, F)");
    static_assert(is_convertible_v<decltype(id), function<F()>>, "id must work as F()");

   public:
    DualSegmentTree() : DualSegmentTree(0) {}
    explicit DualSegmentTree(int n) : DualSegmentTree(vector<F>(n, id())) {}
    explicit DualSegmentTree(vector<F> f) : n(int(f.size())) {
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

        for (int i = h; i >= 1; --i) {
            if (((l >> i) << i) != l) push(l >> i);
            if (((r >> i) << i) != r) push((r - 1) >> i);
        }

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
    void push(int k) {
        apply(2 * k, act[k]);
        apply(2 * k + 1, act[k]);
        act[k] = id();
    }

    int ceil_log2(int x) {
        int res = 0;
        while ((1 << res) < x) ++res;
        return res;
    }
};