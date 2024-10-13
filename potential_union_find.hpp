template <class S>
class PotentialUnionFind {
   public:
    PotentialUnionFind(int n_) : n(n_), par(n_), d(n_) { iota(par.begin(), par.end(), 0); }

    int root(int i) {
        assert(0 <= i && i < n);
        if (par[i] == i) return i;
        int p0 = root(par[i]);
        d[i] += d[par[i]];  // U[p0] - U[i] = (U[p0] - U[par[i]]) + (U[par[i]] - U[i])
        return par[i] = p0;
    }

    // U[i] - U[j] <- x
    bool unite(int i, int j, S x) {
        assert(0 <= i && i < n);
        assert(0 <= j && j < n);
        int r_i = root(i), r_j = root(j);
        if (r_i == r_j) return d[j] - d[i] == x;
        par[r_j] = r_i;
        d[r_j] = d[i] + x - d[j];
        //   U[par[r_j]] - U[r_j]
        // = U[r_i] - U[r_j]
        // = (U[r_i] - U[i]) + (U[i] - U[j]) - (U[r_j] - U[j])
        return true;
    }

    // U[i] - U[j]
    optional<S> delta(int i, int j) {
        assert(0 <= i && i < n);
        assert(0 <= j && j < n);
        int r_i = root(i), r_j = root(j);
        if (r_i != r_j) return nullopt;
        return d[j] - d[i];
    }

   private:
    int n;
    vector<int> par;
    vector<S> d;  // U[par[i]] - U[i]
};