#include "bit_vector.hpp"

template <typename T, typename W>
class CompressedWaveletMatrixFenwickTree {
    class WaveletMatrix {
        int n, lg;
        vector<BitVector> bits;
        vector<int> mid;
        vector<vector<W>> sum;

       public:
        WaveletMatrix() = default;

        WaveletMatrix(vector<int> v, vector<W> w) : n(v.size()) {
            const int ma = *max_element(v.begin(), v.end());
            lg = 0;
            while ((1 << lg) <= ma + 1) ++lg;
            bits.assign(lg, n + 1);
            mid.resize(lg);
            sum.resize(lg, vector<W>(n));
            vector<int> l(n), r(n), ord(n);  // for sort
            iota(ord.begin(), ord.end(), 0);
            for (int i = lg - 1; i >= 0; --i) {
                int li = 0, ri = 0;
                for (int j = 0; j < n; ++j) {
                    if (v[ord[j]] >> i & 1) {
                        bits[i].set(j);
                        r[ri++] = ord[j];
                    } else
                        l[li++] = ord[j];
                }
                mid[i] = li;
                bits[i].build();
                ord.swap(l);
                for (int j = 0; j < ri; ++j) ord[j + li] = r[j];
                for (int j = 0; j < n; ++j) sum[i][j] = w[ord[j]];
                for (int j = 1; j <= n; ++j) {
                    const int k = j + (j & (-j));
                    if (k <= n) sum[i][k - 1] += sum[i][j - 1];
                }
            }
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

        void add(int p, W w) {
            for (int i = lg - 1; i >= 0; --i) {
                const bool b = bits[i][p];
                p = bits[i].rank(p, b);
                if (b) p += mid[i];
                add_inner(i, p, w);
            }
        }

        W range_sum(int l, int r, int upper) {
            check(l, r);
            W res = 0;

            // e.g. upper == 1011
            // -> sum 0xxx + sum 100x + sum 1010
            for (int i = lg - 1; i >= 0; --i) {
                const bool b = upper >> i & 1;
                if (b) {
                    res += sum_inner(i, bits[i].rank0(r)) - sum_inner(i, bits[i].rank0(l));
                }
                tie(l, r) = nxt(l, r, b, i);
            }
            return res;
        }
        W range_sum(int l, int r, int lower, int upper) { return range_sum(l, r, upper) - range_sum(l, r, lower); }

       private:
        pair<int, int> nxt(int l, int r, bool b, int i) { return {bits[i].rank(l, b) + mid[i] * b, bits[i].rank(r, b) + mid[i] * b}; }

        void check(int &l, int &r) {
            if (l < 0) l = 0;
            if (r > n) r = n;
        }

        void add_inner(int i, int p, W w) {
            for (int x = p + 1; x <= n; x += (x & (-x))) sum[i][x - 1] += w;
        }
        W sum_inner(int i, int r) {
            W res = 0;
            for (int x = r; x > 0; x -= (x & -x)) res += sum[i][x - 1];
            return res;
        }
    };

    WaveletMatrix wm;
    vector<T> y;

   public:
    CompressedWaveletMatrixFenwickTree(vector<T> v, vector<W> w) : y(v) {
        sort(y.begin(), y.end());
        y.erase(unique(y.begin(), y.end()), y.end());
        const int n = v.size();
        vector<int> t(n);
        for (int i = 0; i < n; ++i) t[i] = get(v[i]);
        wm = WaveletMatrix(t, w);
    }

    T access(int p) { return y[wm.access(p)]; }
    T operator[](int p) { return access(p); }

    void add(int p, W w) { wm.add(p, w); }

    // sum w[j] for l <= j < r && v[j] < upper
    W range_sum(int l, int r, T upper) { return wm.range_sum(l, r, get(upper)); }

    // sum w[j] for l <= j < r && lower <= v[j] < upper
    W range_sum(int l, int r, T lower, T upper) { return wm.range_sum(l, r, get(lower), get(upper)); }

   private:
    int get(T x) { return lower_bound(y.begin(), y.end(), x) - y.begin(); }
};