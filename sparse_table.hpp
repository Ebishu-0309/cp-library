template <typename S, auto op>
class SparseTable {
    static_assert(std::is_convertible_v<decltype(op), std::function<S(S, S)>>, "op must work as S(S, S)");

   public:
    explicit SparseTable(const vector<S>& v) : n(v.size()), log_table(n + 1) {
        for (int i = 2; i < n + 1; ++i) {
            log_table[i] = log_table[i >> 1] + 1;
        }

        table.resize(log_table[n] + 1, vector<S>(n));

        for (int i = 0; i < n; ++i) {
            table[0][i] = v[i];
        }

        for (int i = 1; (1 << i) <= n; ++i) {
            for (int j = 0; j + (1 << i) <= n; ++j) {
                table[i][j] = op(table[i - 1][j], table[i - 1][j + (1 << (i - 1))]);
            }
        }
    }

    S prod(int l, int r) const {
        const int k = log_table[r - l];

        return op(table[k][l], table[k][r - (1 << k)]);
    }

   private:
    int n;

    vector<int> log_table;
    vector<vector<S>> table;
};