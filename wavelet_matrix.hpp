#include "bit_vector.hpp"

template <typename T>
class CompressedWaveletMatrix {
    class WaveletMatrix {
        int n, ma, lg;
        vector<BitVector> bits;
        vector<int> mid, st;

       public:
        WaveletMatrix() = default;

        WaveletMatrix(vector<int> v) : n(v.size()), ma(*max_element(v.begin(), v.end())) {
            lg = 0;
            while ((1 << lg) < ma + 1) ++lg;
            bits.assign(lg, n + 1);
            mid.resize(lg);
            vector<int> l(n), r(n);  // for sort
            for (int i = lg - 1; i >= 0; --i) {
                int li = 0, ri = 0;
                for (int j = 0; j < n; ++j) {
                    if (v[j] >> i & 1) {
                        bits[i].set(j);
                        r[ri++] = v[j];
                    } else
                        l[li++] = v[j];
                }
                mid[i] = li;
                bits[i].build();
                v.swap(l);
                for (int j = 0; j < ri; ++j) v[j + li] = r[j];
            }
            st.assign(ma + 1, -1);
            for (int i = 0; i < n; ++i)
                if (st[v[i]] == -1) st[v[i]] = i;
        }

        int access(int p) {
            int res = 0;
            for (int i = lg - 1; i >= 0; --i) {
                const bool b = bits[i][p];
                p = bits[i].rank(p, b);
                if (b) res |= 1 << i, p += mid[i];
            }
            return res;
        }
        int operator[](int p) { return access(p); }

        int rank(int x, int r) {
            for (int i = lg - 1; i >= 0; --i) {
                const bool b = x >> i & 1;
                r = mid[i] * b + bits[i].rank(r, b);
            }
            return r - st[x];
        }
        int rank(int x, int l, int r) { return rank(x, r) - rank(x, l); }

        int kth_smallest(int l, int r, int k) {
            check(l, r);
            int res = 0;
            for (int i = lg - 1; i >= 0; --i) {
                const int cnt = bits[i].rank0(r) - bits[i].rank0(l);
                const bool b = cnt <= k;
                if (b) {
                    res |= 1 << i;
                    k -= cnt;
                }
                tie(l, r) = nxt(l, r, b, i);
            }
            return res;
        }

        int kth_largest(int l, int r, int k) { return kth_smallest(l, r, r - l - k - 1); }

        int range_freq(int l, int r, int upper) {
            check(l, r);
            int res = 0;
            for (int i = lg - 1; i >= 0; --i) {
                const bool b = upper >> i & 1;
                if (b) {
                    res += bits[i].rank0(r) - bits[i].rank0(l);
                }
                tie(l, r) = nxt(l, r, b, i);
            }
            return res;
        }
        int range_freq(int l, int r, int lower, int upper) { return range_freq(l, r, upper) - range_freq(l, r, lower); }

       private:
        pair<int, int> nxt(int l, int r, bool b, int i) { return {bits[i].rank(l, b) + mid[i] * b, bits[i].rank(r, b) + mid[i] * b}; }

        void check(int& l, int& r) {
            if (l < 0) l = 0;
            if (r > n) r = n;
        }
    };

    WaveletMatrix wm;
    vector<T> y;

   public:
    CompressedWaveletMatrix(vector<T> v) : y(v) {
        sort(y.begin(), y.end());
        y.erase(unique(y.begin(), y.end()), y.end());
        const int n = v.size();
        vector<int> t(n);
        for (int i = 0; i < n; ++i) t[i] = get(v[i]);
        wm = WaveletMatrix(t);
    }

    T access(int p) { return y[wm.access(p)]; }
    T operator[](int p) { return access(p); }

    int count(T x, int r) {
        int p = get(x);
        if (p == y.size() || y[p] != x) return 0;
        return wm.rank(p, r);
    }
    int count(T x, int l, int r) { return count(x, r) - count(x, l); }

    // k=0: min
    T kth_smallest(int l, int r, int k) { return y[wm.kth_smallest(l, r, k)]; }

    // k=0: max
    T kth_largest(int l, int r, int k) { return y[wm.kth_largest(l, r, k)]; }

    int range_freq(int l, int r, T upper) { return wm.range_freq(l, r, get(upper)); }
    int range_freq(int l, int r, T lower, T upper) { return wm.range_freq(l, r, get(lower), get(upper)); }

   private:
    int get(T x) { return lower_bound(y.begin(), y.end(), x) - y.begin(); }
};