#include "../sparse_table.hpp"

class LCASolverRMQ {
   public:
    LCASolverRMQ() = default;

    template <typename Cost>
    LCASolverRMQ(Tree<Cost> &tree) {
        build(tree);
    }

    template <typename Cost>
    void build(Tree<Cost> &tree) {
        if (!tree.built) {
            tree.build();
        }
        int n = tree.n;
        in = tree.in;
        vector<S> s(2 * n);
        if (*max_element(tree.depth.begin(), tree.depth.end()) > INT_MAX) {
            Tree<int> t(n);
            for (int i = 0; i < n; ++i) {
                t.g[i].reserve(tree.g[i].size());
                for (auto [u, c] : tree.g[i]) {
                    t.add_edge(i, u);
                }
            }
            t.build();
            for (int i = 0; i < 2 * n; ++i) {
                s[i].first = t.depth[t.ord_inv[i]];
                unsigned int edge = t.es[i];
                if (edge >> 31 & 1) {
                    s[i].second = t.par[~edge];
                } else {
                    s[i].second = t.par[edge];
                }
            }
        } else {
            for (int i = 0; i < 2 * n; ++i) {
                s[i].first = tree.depth[tree.ord_inv[i]];
                unsigned int edge = tree.es[i];
                if (edge >> 31 & 1) {
                    s[i].second = tree.par[~edge];
                } else {
                    s[i].second = tree.par[edge];
                }
            }
        }
        sp = decltype(sp)(s);
    }

    int lca(int u, int v) {
        if (in[u] > in[v]) swap(u, v);
        return sp.prod(in[u] + 1, in[v] + 1).second;
    }

   private:
    vector<int> in;
    using S = pair<int, int>;
    static S op(S a, S b) { return min(a, b); }
    SparseTable<S, op> sp;
};