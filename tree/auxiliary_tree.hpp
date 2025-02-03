#include "lca_by_rmq.hpp"

template <typename Cost>
class AuxiliaryTreeSolver {
   public:
    AuxiliaryTreeSolver(Tree<Cost> &tree) { init(tree); }

    void init(Tree<Cost> &tree) {
        if (!tree.built) {
            tree.build();
        }
        n = tree.n;
        in = tree.in;
        index_backet.resize(n, -1);
        depth = tree.depth;
        lca_solver.build(tree);
    }

    pair<vector<tuple<int, int, Cost>>, vector<bool>> aux(vector<int> x) {
        int k = x.size();
        ranges::sort(x, ranges::less{}, [&](int i) { return in[i]; });
        vector<pair<int, int>> edge_list;
        edge_list.reserve(2 * k);
        stack<int> st;
        st.push(x[0]);
        for (int i = 1; i < k; ++i) {
            int w = lca_solver.lca(x[i - 1], x[i]);
            while (!st.empty() && depth[w] <= depth[st.top()]) {
                int u = st.top();
                st.pop();
                if (st.empty() || depth[st.top()] < depth[w]) {
                    if (w != u) {
                        edge_list.emplace_back(w, u);
                    }
                } else {
                    if (st.top() != u) {
                        edge_list.emplace_back(st.top(), u);
                    }
                }
            }
            st.push(w);
            st.push(x[i]);
        }
        int root = -1;
        while (!st.empty()) {
            int u = st.top();
            root = u;
            st.pop();
            if (!st.empty()) {
                if (st.top() != u) edge_list.emplace_back(st.top(), u);
            }
        }

        int k2 = edge_list.size() + 1;
        vector<int> v(k2);
        v[0] = root;
        for (int i = 0; i < k2 - 1; ++i) v[i + 1] = edge_list[i].second;
        for (int i = 0; i < k2; ++i) {
            index_backet[v[i]] = i;
        }
        vector<bool> type(k2);
        for (int i = 0; i < k; ++i) {
            type[index_backet[x[i]]] = true;
        }
        vector<tuple<int, int, Cost>> res(k2 - 1);
        for (int i = 0; i < k2 - 1; ++i) {
            get<0>(res[i]) = index_backet[edge_list[i].first];
            get<1>(res[i]) = index_backet[edge_list[i].second];
            get<2>(res[i]) = depth[edge_list[i].second] - depth[edge_list[i].first];
        }
        return {res, type};
    }

    int n;

   private:
    vector<int> in;
    vector<Cost> depth;
    LCASolverRMQ lca_solver;
    vector<int> index_backet;
};
