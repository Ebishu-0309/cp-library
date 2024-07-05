template <typename T>
class CumulativeSum2D {
   private:
    vector<vector<T>> dat;

   public:
    CumulativeSum2D() = default;

    explicit CumulativeSum2D(size_t n) : dat(n + 1, vector<T>(n + 1)) {}

    CumulativeSum2D(size_t h, size_t w) : dat(h + 1, vector<T>(w + 1)) {}

    CumulativeSum2D(const vector<vector<T>>& vec) {
        const size_t h = vec.size(), w = vec.front().size();

        dat.resize(h + 1, vector<T>(w + 1));

        for (size_t i = 0; i < h; ++i) {
            for (size_t j = 0; j < w; ++j) {
                dat[i + 1][j + 1] = dat[i][j + 1] + dat[i + 1][j] - dat[i][j] + vec[i][j];
            }
        }
    }

    void add(int h, int w, int v) { dat[h + 1][w + 1] += v; }

    void build() {
        const size_t h = dat.size() - 1, w = dat.front().size() - 1;
        for (size_t i = 0; i < h; ++i) {
            for (size_t j = 0; j < w; ++j) {
                dat[i + 1][j + 1] = dat[i][j + 1] + dat[i + 1][j] - dat[i][j];
            }
        }
    }

    // [0, h) x [0, w)
    T sum(int h, int w) const { return sum(0, 0, h, w); }

    // [h1, h2) x [w1, w2)
    T sum(int h1, int w1, int h2, int w2) const { return dat[h2][w2] - dat[h1][w2] - dat[h2][w1] + dat[h1][w1]; }
};